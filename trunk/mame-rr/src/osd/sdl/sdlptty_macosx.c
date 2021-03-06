//============================================================
//
//  sdlptty_macos.c - SDL psuedo tty access functions
//
//  Copyright (c) 1996-2010, Nicola Salmoria and the MAME Team.
//  Visit http://mamedev.org for licensing and usage restrictions.
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <util.h>
#include <fcntl.h>
#include <errno.h>

#include "sdlfile.h"

const char *sdlfile_ptty_identifier  = "/dev/pty";

file_error sdl_open_ptty(const char *path, UINT32 openflags, osd_file **file, UINT64 *filesize)
{
	int master;
	int aslave;
	char name[100];

	if( openpty(&master, &aslave, name, NULL, NULL) >= 0 )
	{
		printf("Slave of device %s is %s\n", path, name );
		fcntl(master, F_SETFL, O_NONBLOCK);
		(*file)->handle = master;
		*filesize = 0;
	}
	else
	{
		return FILERR_ACCESS_DENIED;
	}

	return FILERR_NONE;
}

file_error sdl_read_ptty(osd_file *file, void *buffer, UINT64 offset, UINT32 count, UINT32 *actual)
{
	ssize_t result;

	result = read(file->handle, buffer, count);

	if (result < 0)
	{
		return error_to_file_error(errno);
	}

	if (actual != NULL )
	{
		*actual = result;
	}

	return FILERR_NONE;
}

file_error sdl_write_ptty(osd_file *file, const void *buffer, UINT64 offset, UINT32 count, UINT32 *actual)
{
	UINT32 result;
	result = write(file->handle, buffer, count);

	if (result < 0)
	{
		return error_to_file_error(errno);
	}

	if (actual != NULL )
	{
		*actual = result;
	}

	return FILERR_NONE;
}

file_error sdl_close_ptty(osd_file *file)
{
	close(file->handle);
	osd_free(file);

	return FILERR_NONE;
}
