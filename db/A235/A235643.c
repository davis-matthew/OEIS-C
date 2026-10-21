/*
 * A235643  Total number of sides of a tetraflake-like fractal after n
 *          iterations, a(1) = 16 (see comments).
 * https://oeis.org/A235643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)-7*a(n-2), a(1) = 16, a(2) = 68
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values, 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return 0;   /* undefined for n < 1, but safe return */

    /* Ensure cache can hold at least a(2) */
    if (cache_size < 2) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_size + 1));
        if (!cache) exit(1);
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)68;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* Expand cache to size n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) exit(1);
    cache = new_cache;

    /* Compute values from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)16;
        else if (i == 2)
            cache[i] = (seq_t)68;
        else
            cache[i] = 6 * cache[i - 1] - 7 * cache[i - 2];
    }

    cache_size = n;
    return cache[n];
}
