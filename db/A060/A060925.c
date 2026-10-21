/*
 * A060925  a(n) = 2*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 4.
 * https://oeis.org/A060925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation of already computed terms. */
static seq_t *cache      = NULL;   /* pointer to the allocated array   */
static idx_t  cache_size = 0;      /* number of elements actually stored */
static size_t cache_cap  = 0;      /* size of the allocated storage     */

seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        /* Start with capacity for two terms. */
        cache_cap = 2;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)   /* allocation failure – return zero */
            return (seq_t)0;

        /* Base values of the recurrence. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_size = 2;
    }

    /* Return already known values directly. */
    if (n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* Ensure the underlying storage is large enough. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            /* Double the capacity to keep amortised linear growth. */
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure – return zero */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to the requested index inclusive. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[(size_t)(i - 1)]
                  + (seq_t)3 * cache[(size_t)(i - 2)];
        cache[(size_t)i] = val;
    }
    cache_size = n + 1;   /* we have now stored terms 0 … n */

    return cache[(size_t)n];
}
