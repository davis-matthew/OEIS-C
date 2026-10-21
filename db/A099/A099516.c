/*
 * A099516  A transform of the Pell numbers.
 * https://oeis.org/A099516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A099516:  a(n) = 2*a(n-1)+a(n-2)-2*a(n-3)+2*a(n-4)-a(n-6) for n>=6
 * Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=5, a(4)=10, a(5)=23, a(6)=50
 * This implementation memoises computed terms in a dynamically sized array
 * so that calls to a() in any order remain O(n) in total.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache      = NULL;   /* array of computed a(i) */
    static size_t cache_cap  = 0;      /* allocated capacity of cache */
    static size_t cache_len  = 0;      /* number of valid entries in cache */

    /* base values for n=0..6 (sentinel for fast access) */
    static const seq_t base[7] = {
        (seq_t)0,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)2,  /* a(2) */
        (seq_t)5,  /* a(3) */
        (seq_t)10, /* a(4) */
        (seq_t)23, /* a(5) */
        (seq_t)50  /* a(6) */
    };

    /* Directly return the first seven terms if requested */
    if (n < 7u)
        return base[n];

    /* Ensure the cache array has room for a(n) and any intermediate values */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – fall back to zero */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute any missing terms in the cache */
    if (cache_len < (size_t)(n + 1)) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 7u)
                cache[i] = base[i];
            else {
                /* 2*a(i-1)+a(i-2)-2*a(i-3)+2*a(i-4)-a(i-6) */
                seq_t term = cache[i - 1] * (seq_t)2 + cache[i - 2]
                           + cache[i - 4] * (seq_t)2
                           - (cache[i - 3] * (seq_t)2 + cache[i - 6]);
                cache[i] = term;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
