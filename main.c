#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE  (sizeof (struct inotify_event))
#define EVENT_BUF_LEN     (1024 * (EVENT_SIZE + 16))

int main() {
    int fd, wd;
    char buffer[EVENT_BUF_LEN];
    
    // Initialize the inotify instance
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return EXIT_FAILURE;
    }
    
    // Add a watch to the directory you want to monitor
    wd = inotify_add_watch(fd, "./", IN_MODIFY | IN_CREATE | IN_DELETE);
    
    if (wd == -1) {
        perror("inotify_add_watch");
        return EXIT_FAILURE;
    }
    
    printf("Watching for file changes...\n");
    
    while (1) {
        int length, i = 0;
        
        // Read events from the inotify instance
        length = read(fd, buffer, EVENT_BUF_LEN);
        
        if (length < 0) {
            perror("read");
            return EXIT_FAILURE;
        }
        
        // Process events
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            
            if (event->mask & IN_CREATE) {
                printf("File %s created.\n", event->name);
            }
            if (event->mask & IN_DELETE) {
                printf("File %s deleted.\n", event->name);
            }
            if (event->mask & IN_MODIFY) {
                printf("File %s modified.\n", event->name);
            }
            
            i += EVENT_SIZE + event->len;
        }
    }
    
    // Close the inotify instance (this code won't be reached in this example)
    close(fd);
    
    return EXIT_SUCCESS;
}
