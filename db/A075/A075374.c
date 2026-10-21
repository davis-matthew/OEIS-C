/*
 * A075374  a(n+2) = n*a(n+1) - a(n), with a(1)=1, a(2)=2.
 * https://oeis.org/A075374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n+2) = n*a(n+1) - a(n); a(1)=1, a(2)=2
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A075374 using an iterative recurrence with memoisation. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[i] holds a(i) for 1 ≤ i ≤ cache_size */
    static seq_t *cache = NULL;          /* 1‑based array */
    static idx_t cache_size = 0;         /* current allocated size (highest index stored) */

    /* Initialise cache with base cases if first call */
    if (cache_size == 0) {
        cache_size = 2;                  /* we need indices 1 and 2 */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory: return 0 as a fallback */
        cache[1] = 1;
        cache[2] = 2;
    }

    /* If requested n is already cached, return it */
    if (n <= cache_size)
        return cache[n];

    /* Expand cache to accommodate n */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (new_size + 1) * sizeof(seq_t));
        if (!new_cache) return 0;        /* out of memory: fallback */
        cache = new_cache;
        /* Compute new terms using recurrence */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* a(k) = (k-2)*a(k-1) - a(k-2) */
            cache[k] = (seq_t)(k - 2) * cache[k - 1] - cache[k - 2];
        }
        cache_size = n;
    }
    return cache[n];
}
