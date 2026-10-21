/*
 * A237377  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 336 * a(n-4) for n>5 with initial values a(1)=81,
 *            a(2)=344, a(3)=1472, a(4)=6368, a(5)=27280
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>   /* for free() used by free_cache() */

/* make sure the helper functions for dynamic allocation are available */
#include "../utils/alloc_cache.h"   /* provides alloc_cache() */
#include "../utils/free_cache.h"   /* provides free_cache() */

seq_t a(idx_t n)
{
    /* Hard‑coded initial segment of the sequence. */
    static const seq_t init[6] = {0, 81, 344, 1472, 6368, 27280};

    if (n <= 5)
        return init[n];

    /* Static cache reused between calls. */
    static seq_t *cache = NULL;
    static idx_t cache_cap  = 0;  /* number of elements allocated */
    static idx_t cache_last = 0;  /* highest index that has a value */

    /* Expand the cache if it does not cover the requested index. */
    if (cache == NULL || n + 1 > cache_cap) {
        /* Allocate a new, larger array. */
        seq_t *new_cache = alloc_cache(n + 1);
        if (cache != NULL) {
            /* Free the old array. */
            free_cache(cache);
        }
        cache      = new_cache;
        cache_cap  = n + 1;
        cache_last = 0;          /* values will be recomputed below */
    }

    /* Initialise the first five entries if they are not already done. */
    for (idx_t i = 1; i <= 5 && i < cache_cap; ++i)
        cache[i] = init[i];

    /* Compute any missing entries. */
    idx_t start = cache_last + 1;
    if (start < 6)          /* we need at least the five initial values */
        start = 6;

    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 336 * a(i-4).  The cast ensures the left operand is
           of type seq_t in the multiplication. */
        cache[i] = (seq_t)336 * cache[i - 4];
    }

    cache_last = n;   /* all indices up to n are now valid */

    return cache[n];
}
