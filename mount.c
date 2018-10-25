// mini mount program so you can create mounts as a non-root user
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mount.h>

char *get_opt_arg(int argc, char **argv, int *arg_index)
{
  (*arg_index)++;
  if (*arg_index >= argc) {
    fprintf(stderr, "option '%s' requires an argument\n", argv[(*arg_index) - 1]);
    exit(1);
  }
  return argv[*arg_index];
}

void usage()
{
  printf("Usage: mount [-t <type>] [-o <options>] source target\n");
}
int main(int argc, char *argv[])
{
  argc--;
  argv++;
  char *type = NULL;
  char *options = NULL;
  {
    int old_argc = argc;
    argc = 0;
    int arg_index = 0;
    for (; arg_index < old_argc; arg_index++) {
      char *arg = argv[arg_index];
      if (arg[0] != '-')
        argv[argc++] = arg;
      else if (0 == strcmp(arg, "-t"))
        type = get_opt_arg(old_argc, argv, &arg_index);
      else if (0 == strcmp(arg, "-o"))
        options = get_opt_arg(old_argc, argv, &arg_index);
      else {
        fprintf(stderr, "Error: unknown option '%s'\n", arg);
        return 1;
      }
    }
  }
  if (argc != 2) {
    fprintf(stderr, "Error: expected 2 non-option argc but got %d\n", argc);
    return 1;
  }
  char *source = argv[0];
  char *target = argv[1];
  int result = mount(source, target, type, 0, options);
  if (result) {
    fprintf(stderr, "Error: mount failed: errno:%d (%s)\n", errno, strerror(errno));
    return result;
  }
  return 0;
}
