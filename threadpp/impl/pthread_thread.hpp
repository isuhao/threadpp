//
//  pthread_thread.cpp
//  threadpp
//
//  Created by Melo Yao on 1/15/15.
//  Copyright (c) 2015 Melo Yao. All rights reserved.
//

#ifndef __threadpp__pthread_thread__hpp__
#define __threadpp__pthread_thread__hpp__

#include <errno.h>
#include "../threadpp_assert.h"
#include <cstring>
#include <cmath>
#include <unistd.h>
namespace threadpp
{
    inline pthread_thread::id_type pthread_thread::null_id()
    {
        return 0;
    }
    
    inline void* pthread_thread::pthread_fp_delegate(void* ctx)
    {
        pthread_thread::pthread_context* pctx =static_cast<pthread_thread::pthread_context*>(ctx);
        pctx->fp(pctx->context);
        return NULL;
    }
    
    inline pthread_thread::pthread_thread(runnable r,void* t)
    {
        _context.fp = r;
        _context.context = t;
        int code = pthread_create(&_thread, NULL, pthread_thread::pthread_fp_delegate, &_context);
        ASSERT(code==0,"create thread failed,error:%s",strerror(code));
    }
    
//    pthread_thread::pthread_thread(runnable r,void* t,float priority)
//    {
//        _context.fp = r;
//        _context.context = t;
//        pthread_attr_t tattr;
//        pthread_attr_init(&tattr);
//        struct sched_param schp;
//        int policy = SCHED_FIFO;
//        pthread_attr_getschedpolicy(&tattr, &policy);
//        pthread_attr_getschedparam(&tattr, &schp);
//        float pr =fminf(1.0f,fmaxf(0.0f, priority));
//        schp.sched_priority = sched_get_priority_min(policy) + pr*(sched_get_priority_max(policy) - sched_get_priority_min(policy));
//        pthread_attr_setschedparam(&tattr, &schp);
//        int code = pthread_create(&_thread, &tattr, pthread_thread::pthread_fp_delegate, &_context);
//        ASSERT(code==0,"create thread failed,error:%s",strerror(code));
//        pthread_attr_destroy(&tattr);
//    }
    
    inline pthread_thread::~pthread_thread()
    {
        ASSERT(_thread == 0,"%s","must join or detach a thread before destructing it");
    }
    
    inline void pthread_thread::join()
    {
        void* ret = NULL;
        int code = pthread_join(_thread, &ret);
        _thread = 0;
        ASSERT(code==0,"join thread failed,error:%s",strerror(code));
    }
    
    inline void pthread_thread::detach()
    {
        int code = pthread_detach(_thread);
        _thread = 0;
        ASSERT(code==0,"join thread failed,error:%s",strerror(code));

    }
    
    inline bool pthread_thread::is_equal(const pthread_thread& t) const
    {
        return pthread_equal(_thread, t._thread);
    }
    
    inline void pthread_thread::sleep(unsigned long millisecs)
    {
        usleep((useconds_t)millisecs*1000);
    }
    
    inline pthread_thread::id_type pthread_thread::get_id() const
    {
#ifdef __APPLE__
        __uint64_t tid;
        return pthread_threadid_np(_thread, &tid);
        return tid;
#else
        return (unsigned long long)(_thread);
#endif
    }
    
    inline pthread_thread::id_type pthread_thread::current_thread_id()
    {
#ifdef __APPLE__
        __uint64_t tid;
        pthread_threadid_np(pthread_self(), &tid);
        return tid;
#else
        return (unsigned long long)(pthread_self());
#endif
    }
}
#endif