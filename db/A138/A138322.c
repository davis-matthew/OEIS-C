/*
 * A138322  a(n) = 5*a(n-1) + 10*a(n-2).
 * https://oeis.org/A138322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 10*a(n-2), a(0) = 1, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)15;

    /* Static cache for memoisation across calls */
    static seq_t *cache          = NULL; /* array of a(i) values */
    static idx_t  cache_size     = 0;     /* allocated array size          */
    static idx_t  cached_n       = 1;     /* highest index stored in cache */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                 /* failed allocation */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
    }

    /* If the requested index is already cached, return it */
    if (n <= cached_n) return cache[n];

    /* Grow and compute new values up to index n */
    while (cached_n < n) {
        /* Compute a(cached_n+1) = 5*a(cached_n) + 10*a(cached_n-1) */
        seq_t next = (seq_t)5 * cache[cached_n] + (seq_t)10 * cache[cached_n - 1];

        /* Ensure the array has space for the new element */
        if (cached_n + 2 > cache_size) {
            idx_t new_size = cache_size * 2;
            if (new_size < cached_n + 2) new_size = cached_n + 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) abort();           /* failed reallocation */
            cache = tmp;
            cache_size = new_size;
        }

        /* Store the computed value and advance */
        cache[cached_n + 1] = next;
        cached_n++;
    }

    return cache[n];
}
