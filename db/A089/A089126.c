/*
 * A089126  a(n+2) = F(n+1)*a(n+1) + F(n)*a(n) where F(n) = Fibonacci number
 *          (A000045), a(0) = a(1) = 1.
 * https://oeis.org/A089126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089126:  a(0)=a(1)=1;  a(n+2)=F(n+1)*a(n+1)+F(n)*a(n)
    where F(0)=0, F(1)=1 and F(k)=F(k−1)+F(k−2) (the Fibonacci numbers). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper allocator that works for both C and C++ builds */
static seq_t *alloc_seq_array(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc(sizeof(seq_t) * len);
#endif
}

static void free_seq_array(seq_t *ptr)
{
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

static seq_t *grow_seq_array(seq_t *old, size_t old_len, size_t new_len)
{
#ifdef __cplusplus
    seq_t *newarr = new seq_t[new_len];
    for (size_t i = 0; i < old_len; ++i) newarr[i] = old[i];
    delete[] old;
    return newarr;
#else
    return (seq_t *)realloc(old, sizeof(seq_t) * new_len);
#endif
}

seq_t a(idx_t n)
{
    /* Static arrays holding computed terms and Fibonacci numbers. */
    static seq_t *a_cache = NULL;
    static idx_t a_cache_len = 0;   /* number of terms stored */

    static seq_t *fib_cache = NULL;
    static idx_t fib_cache_len = 0; /* number of Fibonacci numbers stored */

    /* Initialize caches for the very first call. */
    if (n == 0 || n == 1) {
        if (a_cache_len == 0) {
            a_cache = alloc_seq_array(2);
            a_cache[0] = (seq_t)1;
            a_cache[1] = (seq_t)1;
            a_cache_len = 2;
        }
        return a_cache[n];
    }

    if (a_cache_len == 0) {
        a_cache = alloc_seq_array(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache_len = 2;
    }
    if (fib_cache_len == 0) {
        fib_cache = alloc_seq_array(2);
        fib_cache[0] = (seq_t)0;  /* F(0) */
        fib_cache[1] = (seq_t)1;  /* F(1) */
        fib_cache_len = 2;
    }

    /* Extend Fibonacci cache up to index n (needs F(n-1) and F(n-2)). */
    if (fib_cache_len <= n) {
        size_t new_len = (size_t)n + 1;   /* need indices 0..n */
        fib_cache = grow_seq_array(fib_cache, fib_cache_len, new_len);
        for (idx_t k = fib_cache_len; k <= n; ++k) {
            fib_cache[k] = fib_cache[k - 1] + fib_cache[k - 2];
        }
        fib_cache_len = new_len;
    }

    /* Extend a_cache up to index n. */
    if (a_cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        a_cache = grow_seq_array(a_cache, a_cache_len, new_len);
        for (idx_t i = a_cache_len; i <= n; ++i) {
            seq_t prev = a_cache[i - 2];
            seq_t curr = a_cache[i - 1];
            seq_t next = fib_cache[i - 1] * curr + fib_cache[i - 2] * prev;
            a_cache[i] = next;
        }
        a_cache_len = new_len;
    }

    return a_cache[n];
}
