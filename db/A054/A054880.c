/*
 * A054880  a(n) = 3*(9^n - 1)/4.
 * https://oeis.org/A054880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 3*(9^n - 1)/4 Recurrence: a(0) = 0, a(n) = 9*a(n-1) + 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The sequence values are stored in a lazily‑expanded static cache.
   Subsequent calls reuse the computed terms.  The implementation
   works for all supported seq_t types (unsigned integer or
   arbitrary‑precision). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0U;

    /* Initialise the cache with a(0) = 0 if this is the first call */
    if (cache_size == 0U)
    {
        cache_size = 1U;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)                 /* allocation failed – return 0 as fallback */
            return (seq_t)0;
        cache[0] = (seq_t)0;
    }

    /* If the requested term is already in the cache, simply return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1U) * sizeof(seq_t));
    if (!new_cache)               /* allocation failed – return 0 */
        return (seq_t)0;
    cache = new_cache;

    /* Compute the missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* a(i) = 9*a(i-1) + 6 */
        cache[i] = (cache[i - 1] * (seq_t)9) + (seq_t)6;
    }

    cache_size = n + 1U;
    return cache[(size_t)n];
}
