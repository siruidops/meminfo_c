/* 
 * (https://github.com/siruidops/meminfo_c).
 * Copyright (c) 2021 uidops.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h> // sstandard buffered input/output
#include <stdlib.h> // standard library definitions
#include <string.h> //  string operations

const char file_name[] = "/proc/meminfo"; // meminfo file for parse data.

void remove_spaces(char* s) {
    /*
    * remove space and new line from string.
    */
    
    const char* d = s;
    do {
        while (*d == ' ' || *d == '\n') {
            ++d;
        }
    } while (*s++ == *d++);
}

void remove_kB(char *text) {
    /*
    * remove 'kB' from string. 
    */

    int length_string, length_word, temp, ch=0;
    char *word = "kB";
    int i,j;
    
    length_string = strlen(text);
    length_word = strlen(word);

    for (i=0; i<length_string; i++) {
        temp = i;
        
        for (j = 0; j < length_word; j++) {
            if (text[i] == word[j])
                i++;
        }

        ch = i - temp;
        if (ch == length_word) {
            i = temp;
            
            for (j = i; j < (length_string - length_word); j++)
                text[j] = text[j+length_word];
            
            length_string = length_string - length_word;
            text[j]='\0';
        }
    }
}

char *file_search(char *str) {
    /*
    * Search for data and perform some actions on them.
    */

	FILE *fp;
	char temp[512];
    char *message;
	
	fp = fopen(file_name, "r");

	while (fgets(temp, 512, fp) != NULL) {
		if (strstr(temp, str) != NULL) {

            message = malloc(sizeof(temp)*15);
            strcpy(message, temp);

            remove_spaces(message);
            remove_kB(message);

            message = strtok(message, ":");
            message = strtok(NULL, ":");

            return message;
		}
	}

    return NULL;
}


int main() {
    char *total = file_search("MemTotal");
    char *free = file_search("MemFree");
    char *buffers = file_search("Buffers");
    char *cache = file_search("Cached");

    int used = ((atoi(total) - atoi(free)) - (atoi(buffers) + atoi(cache))) * 0.001;
    int avai = atoi(total)*0.001;

    printf("%d MB/%d MB\n", used, avai);

    return 0;

}
