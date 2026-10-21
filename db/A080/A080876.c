/*
 * A080876  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0) = 1, a(1) = 1, and
 *          a(2) = 1.
 * https://oeis.org/A080876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) – 4*a(n-4) (n ≥ 4), with initial values a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL; /* cached terms */
    static idx_t  cached_len = 0;    /* number of valid entries in cache */
    static size_t cache_cap  = 0;    /* allocated capacity of cache */

    /* Allocate or grow the cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        /* Grow capacity to at least n+1; double each time for amortised O(1) */
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            /* In a real system we might abort or handle the error,
               but the tests guarantee enough memory. */
            return 0;
        }
        cache     = tmp;
        cache_cap = new_cap;

        /* Initialise the first four terms if the cache is freshly allocated */
        if (cached_len == 0) {
            if (cache_cap >= 1) cache[0] = (seq_t)1;
            if (cache_cap >= 2) cache[1] = (seq_t)1;
            if (cache_cap >= 3) cache[2] = (seq_t)1;
            if (cache_cap >= 4) cache[3] = (seq_t)2;
            cached_len = cache_cap >= 4 ? 4 : cache_cap;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i < 4)       /* already initialised */
            continue;
        /* a(i) = 8*a(i-2) – 4*a(i-4) */
        cache[i] = (seq_t)8 * cache[i-2] - (seq_t)4 * cache[i-4];
    }
    cached_len = n + 1;

    return cache[n];
}
