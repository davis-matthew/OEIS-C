/*
 * A170748  Expansion of g.f.: (1+x)/(1-28*x).
 * https://oeis.org/A170748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29 * 28^(n-1) for n > 0; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the n‑th term of OEIS sequence A170748.
   It uses a static cache so that repeated calls are fast, even if
   the indices are not monotonic.  The cache is grown only as needed. */
seq_t a(idx_t n)
{
    /* A cache of previously computed terms, initially empty. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* The first term is special. */
    if (n == 0)
        return (seq_t)1;

    /* Size required in the cache. */
    size_t required = (size_t)n + 1;

    /* Allocate or grow the cache when necessary. */
    if (cache == NULL) {
        /* First call: allocate at least size 2 (for indices 0 and 1).   */
        size_t init = required < 2 ? 2 : required;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* failure path – should never happen   */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache_size = init;

        /* If more than two slots are required, fill them. */
        for (size_t i = 2; i < required; ++i)
            cache[i] = cache[i - 1] * (seq_t)28;
        cache_size = required;
    }
    else if (required > cache_size) {
        /* Grow the cache to the needed size. */
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* failure path – should never happen */
        cache = tmp;

        /* Compute the new entries by iterating from the old size. */
        for (size_t i = cache_size; i < required; ++i)
            cache[i] = cache[i - 1] * (seq_t)28;
        cache_size = required;
    }

    return cache[(size_t)n];
}
