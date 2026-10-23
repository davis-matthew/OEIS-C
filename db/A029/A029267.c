/*
 * A029267  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 762, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029267: number of partitions of n into parts 3, 4, 9, and 11
   Computed by dynamic programming with the coin‑change algorithm.
   For each coin value w the array is updated by dp[i] += dp[i-w]
   for i = w … n.  The array is grown lazily so that repeated calls
   with arbitrary indices remain fast. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;          /* cached partition counts */
    static idx_t dp_size = 0;         /* current length of dp array   */
    static const idx_t parts[4] = {3, 4, 9, 11};
    static const size_t parts_cnt = 4;

    /* Expand the cache if necessary */
    if (dp_size == 0 || n >= dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) return 0;      /* allocation failed */
        dp = new_dp;

        /* Recompute all values up to the new maximum */
        for (idx_t i = 0; i < new_size; ++i) dp[i] = 0;
        dp[0] = 1;
        for (size_t p = 0; p < parts_cnt; ++p) {
            idx_t w = parts[p];
            for (idx_t i = w; i < new_size; ++i) {
                dp[i] += dp[i - w];
            }
        }
        dp_size = new_size;
    }

    return dp[n];
}
