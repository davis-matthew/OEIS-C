/*
 * A029058  Expansion of 1/((1-x)(1-x^3)(1-x^9)(1-x^10)).
 * https://oeis.org/A029058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 9 and 10.
 *            Computed with the standard DP: dp[0] = 1; for each part p in
 *            {1,3,9,10}: for i = p .. n: dp[i] += dp[i-p];
 * Range:     u8 n <= 63, u16 n <= 462, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* holds dp[0..max_n] */
static idx_t cache_size = 0;     /* current length of cache (max_n + 1) */
static const idx_t parts[] = {1,3,9,10};
static const size_t PARTS_COUNT = 4;

seq_t a(idx_t n)
{
    /* allocate or extend cache to hold values up to n */
    if (!cache || n + 1 > cache_size) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed */
        cache = new_cache;
        cache_size = new_cap;

        /* initialise all entries to 0, then build the dp table */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        cache[0] = 1;   /* base case */

        for (size_t pidx = 0; pidx < PARTS_COUNT; ++pidx) {
            idx_t p = parts[pidx];
            for (idx_t i = p; i <= n; ++i)
                cache[i] += cache[i - p];
        }
    }
    return cache[n];
}
