/*
 *  $Id$
 *      Author: Jeffrey O. Hill
 *              hill@luke.lanl.gov
 *              (505) 665 1831
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 *
 */

#define epicsAssertAuthor "Jeff Hill johill@lanl.gov"

#include "iocinf.h"
#include "oldAccess.h"

#define epicsExportSharedSymbols
#include "syncGroup.h"

/*
 * ca_sg_create()
 */
extern "C" int epicsShareAPI ca_sg_create ( CA_SYNC_GID *pgid ) // X aCC 361
{
    oldCAC *pcac;
    int caStatus;
    CASG *pcasg;

    caStatus = fetchClientContext ( &pcac );
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = new CASG ( *pcac );
    if ( pcasg ) {
        *pgid = pcasg->getId ();
        return ECA_NORMAL;
    }
    else {
        return ECA_ALLOCMEM;
    }
}

/*
 * ca_sg_delete()
 */
extern "C" int epicsShareAPI ca_sg_delete ( const CA_SYNC_GID gid )
{
    oldCAC *pcac;
    int caStatus;
    CASG *pcasg;

    caStatus = fetchClientContext ( &pcac );
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        return ECA_BADSYNCGRP;
    }

    pcasg->destroy ();

    return ECA_NORMAL;
}

/*
 * ca_sg_block ()
 */
extern "C" int epicsShareAPI ca_sg_block ( const CA_SYNC_GID gid, ca_real timeout )
{
    oldCAC *pcac;
    CASG *pcasg;
    int status;

    status = fetchClientContext (&pcac);
    if ( status != ECA_NORMAL ) {
        return status;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        status = ECA_BADSYNCGRP;
    }
    else {
        status = pcasg->block ( timeout );
    }

    return status;
}

/*
 * ca_sg_reset
 */
extern "C" int epicsShareAPI ca_sg_reset ( const CA_SYNC_GID gid )
{
    oldCAC *pcac;
    CASG *pcasg;
    int caStatus;

    caStatus = fetchClientContext (&pcac);
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        caStatus = ECA_BADSYNCGRP;
    }
    else {
        caStatus = ECA_NORMAL;
        pcasg->reset ();
    }

    return caStatus;
}

/*
 * ca_sg_stat
 */
extern "C" int epicsShareAPI ca_sg_stat ( const CA_SYNC_GID gid )
{
    oldCAC *pcac;
    CASG *pcasg;

    int caStatus = fetchClientContext (&pcac);
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        ::printf ( "Bad Sync Group Id\n");
        return ECA_BADSYNCGRP;
    }

    pcasg->show ( 1000u );

    return ECA_NORMAL;
}

/*
 * ca_sg_test
 */
extern "C" int epicsShareAPI ca_sg_test ( const CA_SYNC_GID gid ) // X aCC 361
{
    oldCAC *pcac;
    CASG *pcasg;
    int caStatus;

    caStatus = fetchClientContext (&pcac);
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        return ECA_BADSYNCGRP;
    }

    if ( pcasg->ioComplete () ) {
        return ECA_IODONE;
    }
    else{
        return ECA_IOINPROGRESS;
    }
}

/*
 * ca_sg_array_put()
 */
extern "C" int epicsShareAPI ca_sg_array_put ( const CA_SYNC_GID gid, chtype type,
    arrayElementCount count, chid pChan, const void *pValue )
{
    oldCAC *pcac;
    CASG *pcasg;
    int caStatus;
    
    caStatus = fetchClientContext ( &pcac );
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        return ECA_BADSYNCGRP;
    }

    return pcasg->put ( pChan, type, 
        static_cast < unsigned > ( count ), pValue );
}

/*
 * ca_sg_array_get()
 */
extern "C" int epicsShareAPI ca_sg_array_get ( const CA_SYNC_GID gid, chtype type,
    arrayElementCount count, chid pChan, void *pValue )
{
    oldCAC *pcac;
    CASG *pcasg;
    int caStatus;

    caStatus = fetchClientContext ( &pcac );
    if ( caStatus != ECA_NORMAL ) {
        return caStatus;
    }

    pcasg = pcac->lookupCASG ( gid );
    if ( ! pcasg ) {
        return ECA_BADSYNCGRP;
    }

    return pcasg->get ( pChan, type, 
        static_cast < unsigned > ( count ), pValue );
}
