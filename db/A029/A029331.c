/*
 * A029331  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A029331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029331: Number of partitions of n into parts 4, 5, 6, and 12.
   Computed by dynamic programming over the coin denominations. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds values up to the largest n computed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    const idx_t denom[4] = {4, 5, 6, 12};

    /* extend cache if required */
    if (n >= cache_size) {
        /* allocate space for all values up to n */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;                 /* allocation failure (unlikely) */
        /* initialise dp array */
        for (idx_t i = 0; i <= n; ++i) new_cache[i] = 0;
        new_cache[0] = 1;                         /* one way to make amount 0 */

        /* classic coin‑change DP: combinations, order irrelevant */
        for (size_t c = 0; c < 4; ++c) {
            idx_t k = denom[c];
            for (idx_t i = k; i <= n; ++i)
                new_cache[i] += new_cache[i - k];
        }

        /* replace old cache */
        free(cache);
        cache = new_cache;
        cache_size = n + 1;
    }

    /* return the desired value */
    return cache[n];
}
