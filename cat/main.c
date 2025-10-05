#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    int c; // variable to hold each character read as an int (needed for EOF)

    // If no file added read from stdin
    // stdin is a predefined global pointer of type FILE * that refers to the standard input (keyboard by default)
    if (argc <= 1) {
        FILE *fd = stdin; // pointer to a FILE structure representing standard input

        // fgetc(fd): reads one character from the stream fd and returns it as int,
        //            or EOF if end-of-file or error.
        // putchar(c): writes one character to standard output and returns that character
        //             (as unsigned char cast to int), or EOF if an error occurs.
        // two checks because:
        //   - first: check for EOF from fgetc (stop reading at end-of-file)
        //   - second: check if putchar failed (error writing to output)
        while ((c = fgetc(fd)) != EOF) {
            if (putchar(c) == EOF) {
                // perror("putchar"): prints "putchar: <system error message>" to stderr
                perror("putchar");
                return 1;
            }
        }

        // ferror(fd): checks if an error occurred on the stream fd (not just EOF)
        if (ferror(fd)) {
            perror("reading from stdin");
            return 1;
        }

        return 0; // finished reading from stdin successfully
    }

    // argc > 1: user passed file names on the command line
    for (int i = 1; i < argc; ++i) {
        // argv[i] is a pointer to the string containing the file name
        const char *filename = argv[i];

        FILE *fd = fopen(filename, "r");

        if (fd == NULL) {

            fprintf(stderr, "Error when opening '%s': ", filename);
            // perror("") prints the system error message corresponding to errno,
            // preceded by whatever string you give. Here you give "", so only the message appears
            perror("");
            // continue jumps to the next iteration of the for-loop,
            // meaning we skip reading this file and move on to the next one
            continue;
        }

        // read characters from the file and write to stdout
        while ((c = fgetc(fd)) != EOF) {
            if (putchar(c) == EOF) {
                perror("putchar");
                fclose(fd);
                return 1;
            }
        }

        // after the loop we check if an actual error occurred while reading
        // (different from just reaching EOF)
        if (ferror(fd)) {
            fprintf(stderr, "Error when reading '%s': ", filename);
            perror("");
            fclose(fd);
            return 1;
        }

        if (fclose(fd) == EOF) {
            fprintf(stderr, "Error when closing '%s': ", filename);
            perror("");
            return 1;
        }
    }

    return 0; // success
}