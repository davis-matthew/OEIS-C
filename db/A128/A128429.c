/*
 * A128429  A linear recurrence sequence: a(n) = a(n-1) + a(n-3) + a(n-5) +
 *          a(n-6).
 * https://oeis.org/A128429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-6), a(0) = a(1) = a(2) =
 *            a(3) = a(4) = a(5) = 1
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial small indices directly */
    if (n < 6ULL)
        return (seq_t)1;

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;        /* pointer to array of a(i) */
    static idx_t cache_cap = 0;        /* allocated capacity */
    static idx_t computed_len = 0;     /* number of terms stored in cache */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* Start with a modest capacity; will grow automatically */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Initialize the first six terms */
        for (idx_t i = 0; i < 6ULL; ++i)
            cache[i] = (seq_t)1;
        computed_len = 6ULL;
    }

    /* Grow cache to accommodate index n, if necessary */
    if (n + 1ULL > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;           /* double until big enough */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < computed_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    while (computed_len <= n) {
        cache[computed_len] =
            cache[computed_len - 1] +
            cache[computed_len - 3] +
            cache[computed_len - 5] +
            cache[computed_len - 6];
        ++computed_len;
    }

    /* Return the requested term */
    return cache[n];
}
