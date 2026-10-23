/*
 * A029122  Expansion of 1/((1-x)(1-x^7)(1-x^9)(1-x^11)).
 * https://oeis.org/A029122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 7, 9, and 11. The
 *            sequence is the coefficient sequence of
 *            1/((1-x)(1-x^7)(1-x^9)(1-x^11)). It is computed with a standard
 *            unbounded knapsack DP
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp      = NULL;
    static size_t current_cap = 0;
    size_t target = (size_t)n + 1;

    /* Grow the table if necessary and recompute all entries. */
    if (target > current_cap) {
        size_t new_cap = target;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        dp = tmp;
        /* initialise entries to 0, then set dp[0] = 1. */
        for (size_t i = 0; i < new_cap; ++i) dp[i] = 0;
        dp[0] = 1;

        /* Prepare the allowed parts. */
        const int parts[4] = {1, 7, 9, 11};
        for (int j = 0; j < 4; ++j) {
            size_t p = (size_t)parts[j];
            for (size_t i = p; i < new_cap; ++i) {
                dp[i] += dp[i - p];
            }
        }
        current_cap = new_cap;
    }

    return dp[n];
}
