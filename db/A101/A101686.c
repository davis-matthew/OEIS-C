/*
 * A101686  a(n) = Product_{i=1..n} (i^2 + 1).
 * https://oeis.org/A101686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (i^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 (empty product) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t alloc_size = 0;
    static idx_t max_cached = (idx_t)-1;   /* indicates not initialized */

    /* initialize cache for n = 0 */
    if (max_cached == (idx_t)-1) {
        /* at least one element (for n = 0) */
        alloc_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * alloc_size);
        cache[0] = (seq_t)1;   /* a(0) */
        max_cached = 0;
    }

    /* if requested n is already cached, return it */
    if (n <= max_cached)
        return cache[n];

    /* enlarge cache to hold up to n */
    size_t needed = (size_t)n + 1;
    if (needed > alloc_size) {
        size_t new_size = alloc_size ? alloc_size : 1U;
        while (new_size < needed)
            new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        alloc_size = new_size;
    }

    /* compute new values iteratively */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i + (seq_t)1;   /* i^2 + 1 */
        cache[i] = cache[i - 1] * term;                /* a(i) = a(i-1)*(i^2+1) */
    }
    max_cached = n;

    return cache[n];
}
