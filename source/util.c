/*
	Copyright (C) 2006 by Jonas Kramer
	Published under the terms of the GNU General Public License (GPL).
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "getln.h"

/*
	Takes the path of a file as argument, reads the file and returns an array
	of strings containing the lines of the file.
*/
char ** slurp(const char * path) {
	char ** content = NULL;
	unsigned items = 0;
	FILE * fd;

	if((fd = fopen(path, "r")) != NULL) {
		char * line = NULL;
		unsigned size = 0;

		while(!feof(fd)) {
			if(!getln(& line, & size, fd))
				continue;

			if(strlen(line) > 1) {
				char * ptr;
				if((ptr = strrchr(line, 10)) != NULL)
					* ptr = (char) 0;

				if(strlen(line) > 0) {
					content = realloc(content, sizeof(char *) * (items + 2));

					assert(content != NULL);

					content[items] = strdup(line);
					assert(content[items] != NULL);

					content[++items] = NULL;
				}
			}
		}

		if(line)
			free(line);

		fclose(fd);
	}

	return content;
}


/*
	Takes an array of strings and returns another array of strings with all
	duplicate strings removed. The old array is purged from memory.
*/
char ** uniq(char ** list) {
	char ** uniqlist = NULL;

	if(list != NULL) {
		int size = 0, n = 0;

		while(list[n] != NULL) {
			if(grep(uniqlist, list[n])) {
				free(list[n]);
				list[n] = NULL;
			} else {
				uniqlist = realloc(uniqlist, (sizeof(char *)) * (size + 2));

				assert(uniqlist != NULL);

				uniqlist[size++] = list[n];
				uniqlist[size] = NULL;
			}

			++n;
		}

		free(list);
	}

	return uniqlist;
}


int grep(char ** list, char * needle) {
	register unsigned x = 0;
	
	if(!needle)
		return 0;

	if(list != NULL) {
		while(list[x] != NULL) {
			if(!strcmp(list[x], needle))
				return !0;
			++x;
		}
	}

	return 0;
}


#ifdef __STRNDUP__

char * strndup(const char * src, size_t len) {
	char * tmp = (char *) malloc(len + 1);

	if(tmp != NULL) {
		strncpy(tmp, src, len);
		tmp[len] = 0;
	}

	return tmp;
}

#endif
