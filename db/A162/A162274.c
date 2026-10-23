/*
 * A162274  a(n) = ((2+sqrt(3))*(4+sqrt(3))^n + (2-sqrt(3))*(4-sqrt(3))^n)/2.
 * https://oeis.org/A162274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2, a(1) = 11,
   a(n) = 8*a(n-1) - 13*a(n-2)   for n > 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static cache of previously computed terms  */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t cache_sz = 0;      /* number of terms stored */

    /* If we have a requested index already cached, return it */
    if (n < cache_sz)
        return cache[n];

    /* Ensure the cache contains at least the first two terms  */
    if (cache_sz < 2) {
        idx_t need = 2;
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (cache_sz == 0) {               /* fresh allocation */
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)11;
        } else if (cache_sz == 1) {        /* previously had a[0] only */
            cache[1] = (seq_t)11;
        }
        cache_sz = need;
    }

    /* If the requested index is already one of the initial terms, finish */
    if (n < cache_sz)
        return cache[n];

    /* Grow the cache to hold values up to n */
    idx_t new_sz = n + 1;
    cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));

    /* Compute missing terms using the recurrence  */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)8;
        seq_t t2 = cache[i - 2] * (seq_t)13;
        cache[i] = t1 - t2;
    }

    cache_sz = new_sz;
    return cache[n];
}
