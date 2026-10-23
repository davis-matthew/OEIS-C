/*
 * A016218  Expansion of 1/((1-x)*(1-4*x)*(1-5*x)).
 * https://oeis.org/A016218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3), n >= 3 Initial values:
 *            a(0) = 1, a(1) = 10, a(2) = 71
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialize cache with base values */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* Out of memory, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)71;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        size_t i = cache_len;
        seq_t val;
        /* Recurrence valid for i >= 3; for i==3 we have the base case in cache */
        if (i >= 3) {
            val = (seq_t)( (seq_t)10 * cache[i-1]
                         - (seq_t)29 * cache[i-2]
                         + (seq_t)20 * cache[i-3] );
        } else {
            /* Should not happen; safety fallback */
            val = (seq_t)0;
        }

        /* Resize cache if necessary (grow by one each iteration, fine for n <= 10000) */
        seq_t *new_cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return 0 to indicate error (context dependent) */
            return (seq_t)0;
        }
        cache = new_cache;
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[(size_t)n];
}
