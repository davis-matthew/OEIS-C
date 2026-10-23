/*
 * A029244  Expansion of 1/((1-x^2)*(1-x^10)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 10, 11 and 12
 *            This implementation uses a dynamic programming recurrence: dp[0]
 *            = 1 for each part p in {2,10,11,12} for i = p .. n dp[i] +=
 *            dp[i-p] The array is cached so repeated calls remain O(1) after
 *            the first computation up to a given n
 * Range:     u8 n <= 137, u16 n <= 989, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;   /* array dp[0..len-1] */
    static idx_t cache_len = 0;   /* number of elements stored */

    if (n >= cache_len) {
        /* Allocate a new array large enough for n */
        idx_t new_len = n + 1;
        /* calloc zero-initialises the array */
        seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */

        /* Base case: only one way to partition 0 */
        new_cache[0] = (seq_t)1;

        /* The fixed set of allowed parts */
        static const idx_t parts[4] = {2, 10, 11, 12};

        /* Fill the DP table up to n */
        for (int pi = 0; pi < 4; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i < new_len; ++i) {
                new_cache[i] += new_cache[i - p];
            }
        }

        /* Replace the old cache */
        free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
