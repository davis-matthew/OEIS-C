/*
 * A052528  Expansion of (1 - x)/(1 - 2*x - 2*x^2 + 2*x^3).
 * https://oeis.org/A052528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=4 * Recurrence: a(n+3) = 2*a(n+2) +
 *            2*a(n+1) - 2*a(n) for n >= 0 * This implements the linear
 *            recurrence for OEIS A052528
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding computed terms up to the largest requested index. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise cache with the first three terms if needed. */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }

    /* If the requested index is already computed, return it. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Extend the cache up to the requested index. */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;

        /* Compute the missing terms using the recurrence. */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     - (seq_t)2 * cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
