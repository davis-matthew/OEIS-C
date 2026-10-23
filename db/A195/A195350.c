/*
 * A195350  Expansion of (1 - 3*x - x^2)/(1 - 4*x + 2*x^3 + x^4).
 * https://oeis.org/A195350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-3) - a(n-4) (for n ≥ 4) Initial values:
 *            a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 10
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of elements already in cache */

    /* Initialise cache with the first four terms on the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* fatal error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)10;
        cache_size = 4;
    }

    /* If the requested index is already cached, just return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to accommodate up to index n */
    size_t newsize = (size_t)(n + 1);
    seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (newcache == NULL) exit(EXIT_FAILURE); /* fatal error */
    cache = newcache;

    /* Compute missing elements using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 4*a(i-1) - 2*a(i-3) - a(i-4) */
        cache[i] = (seq_t)4 * cache[i - 1]
                 - (seq_t)2 * cache[i - 3]
                 -          cache[i - 4];
    }

    cache_size = newsize;
    return cache[n];
}
