/*
 * A097843  First differences of Chebyshev polynomials S(n,123) = A049670(n+1)
 *          with Diophantine property.
 * https://oeis.org/A097843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 123*a(n-1) - a(n-2), a(0) = 1, a(1) = 122
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache for computed values   */
static idx_t cache_size = 0;     /* number of entries stored   */

seq_t a(idx_t n)
{
    /* In the first call initialise the cache with the two seed values */
    if (cache == NULL) {
        /* allocate at least two entries for a(0) and a(1) */
        idx_t init_size = (n < 1 ? 2 : n + 1);
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        cache[0] = (seq_t)1;         /* a(0) */
        if (init_size > 1) cache[1] = (seq_t)122;  /* a(1) */
        cache_size = (init_size > 1 ? 2 : 1);
    }

    /* If the requested index is already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Extend the cache up to n (inclusive) */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_size > cache_size) {
        /* For indices 0 and 1 we already have values.
           New indices start from cache_size up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = 123*a(i-1) - a(i-2) */
            seq_t val = (seq_t)123 * cache[i - 1] - cache[i - 2];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
