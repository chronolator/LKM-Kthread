#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define LICENSE		"GPL"
#define AUTHOR		"Chronolator"
#define DESCRIPTION	"Simple lkm that demonstrates the creation of 2 kernel threads"
#define VERSION 	"0.02"

MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);

static int func_kthread(void *arg) {
    /* Every kthread has a struct task_struct associated with it which is it's identifier.
    * Whenever a thread is schedule for execution, the kernel sets "current" pointer to 
    * it's struct task_struct.
    * current->comm is the name of the command that caused creation of this thread
    * current->pid is the process of currently executing thread 
    */
    printk(KERN_INFO "Func-kthread: %s[PID = %d]\n", current->comm, current->pid);
    return 0;
}

static int __init run_init(void) {
    printk(KERN_INFO "Kthread-init: Initializing module\n");
    
    struct task_struct *ts1;
    struct task_struct *ts2;
    int err;

    printk(KERN_INFO "Starting 2 threads\n");

    /*struct task_struct *kthread_create(int (*threadfn)(void *data), void *data,  *const char *namefmt, ...);
    * This function creates a kernel thread and starts the thread.
    */
    ts1 = kthread_run(func_kthread, NULL, "kthread-1");
    if (IS_ERR(ts1)) {
        printk(KERN_INFO "ERROR: Cannot create kthread ts1\n");
        err = PTR_ERR(ts1);
        ts1 = NULL;
        return err;
    }

    ts2 = kthread_run(func_kthread, NULL, "kthread-2");
    if (IS_ERR(ts2)) {
        printk(KERN_INFO "ERROR: Cannot create kthread ts2\n");
        err = PTR_ERR(ts2);
        ts2 = NULL;
        return err;
    }

    printk(KERN_INFO "Kthread-init: %s[PID = %d]\n", current->comm, current->pid);
    return 0;
}

static void __exit run_exit(void) {
    printk(KERN_INFO "Kthread-exit: Exiting module\n");
}

module_init(run_init);
module_exit(run_exit);
