#include "helpers.h"

int spawn(const char *file, char *const argv[]) {
  pid_t child_id = fork();
  if (child_id == -1) {
    goto ERROR;
  } else if (child_id == 0) { /* Child thread */
    execvp(file, argv);
    goto ERROR;
  } else { /* Parent thread */
    int status;
    wait(&status); /* There is only one child */
    if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
    }
    goto ERROR;
  }

ERROR:
  return -1;
}

ssize_t read_(int fd, void *buf, size_t count) {
  size_t allready_readed = 0;
  while (allready_readed < count) {
    ssize_t readed = read(fd, buf + allready_readed, count - allready_readed);
    if (readed == -1) {
      return readed;
    }
    if (readed == 0) {
      break;
    }
    allready_readed += readed;
  }
  return allready_readed;
}

ssize_t write_(int fd, const void *buf, size_t count) {
  size_t allready_writed = 0;
  while (allready_writed < count) {
    ssize_t writed = write(fd, buf + allready_writed, count - allready_writed);
    if (writed == -1) {
      return writed;
    }
    allready_writed += writed;
  }
  return allready_writed;
}

ssize_t read_until(int fd, void *buf, size_t count, const char delimiter) {
  size_t allready_readed = 0;
  int contains_delimiter = 0;
  while (allready_readed < count && contains_delimiter == 0) {
    ssize_t readed = read(fd, buf + allready_readed, count - allready_readed);
    if (readed == -1) {
      return readed;
    }
    if (readed == 0) {
      break;
    }
    for (size_t i = allready_readed; i < allready_readed + readed; ++i) {
      if (*((char *) buf + i) == delimiter) {
        contains_delimiter = 1;
        break;
      }
    }
    allready_readed += readed;
  }
  return allready_readed;
}
