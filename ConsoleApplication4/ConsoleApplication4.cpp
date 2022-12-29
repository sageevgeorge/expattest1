// ConsoleApplication4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <direct.h>
#include <expat.h>



#ifdef XML_LARGE_SIZE
#  define XML_FMT_INT_MOD "ll"
#else
#  define XML_FMT_INT_MOD "l"
#endif

#ifdef XML_UNICODE_WCHAR_T
#  define XML_FMT_STR "ls"
#else
#  define XML_FMT_STR "s"
#endif

static void XMLCALL
startElement(void *userData, const XML_Char *name, const XML_Char **atts) {
	int i;
	int *const depthPtr = (int *)userData;
	(void)atts;

	for (i = 0; i < *depthPtr; i++)
		putchar('\t');
	printf("%" XML_FMT_STR "\n", name);
	*depthPtr += 1;
}

static void XMLCALL
endElement(void *userData, const XML_Char *name) {
	int *const depthPtr = (int *)userData;
	(void)name;

	*depthPtr -= 1;
}

int
main(void) {
	XML_Parser parser = XML_ParserCreate(NULL);
	int done;
	int depth = 0;
	FILE *infile;
	char* buffer;

	// Get the current working directory:
	if ((buffer = _getcwd(NULL, 0)) == NULL)
		perror("_getcwd error");
	else
	{
		printf("%s \nLength: %zu\n", buffer, strlen(buffer));
		free(buffer);
	}
//	infile = fopen("C:\\Users\\sagee\\source\\repos\\ConsoleApplication4\\infile.txt", "r+");
	infile = fopen("infile.txt", "r+");
	if (infile==NULL)
	{
		printf("bad\n");
		return 1;
	}
	if (!parser) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		return 1;
	}

	XML_SetUserData(parser, &depth);
	XML_SetElementHandler(parser, startElement, endElement);

	do {
		void *const buf = XML_GetBuffer(parser, BUFSIZ);
		if (!buf) {
			fprintf(stderr, "Couldn't allocate memory for buffer\n");
			XML_ParserFree(parser);
			return 1;
		}
		const size_t len = fread(buf, 1, BUFSIZ, infile);
//		const size_t len = fread(buf, 1, BUFSIZ, stdin);

//		if (ferror(stdin)) {
		if (ferror(infile)) {
				fprintf(stderr, "Read error\n");
			XML_ParserFree(parser);
			return 1;
		}

//		done = feof(stdin);
		done = feof(infile);

		if (XML_ParseBuffer(parser, (int)len, done) == XML_STATUS_ERROR) {
			fprintf(stderr,
				"Parse error at line %" XML_FMT_INT_MOD "u:\n%" XML_FMT_STR "\n",
				XML_GetCurrentLineNumber(parser),
				XML_ErrorString(XML_GetErrorCode(parser)));
			XML_ParserFree(parser);
			return 1;
		}
	} while (!done);

	XML_ParserFree(parser);
	return 0;
}

/*
int main()
{
    std::cout << "Hello World!\n";
}
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
