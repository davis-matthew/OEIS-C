/*
 * A029371  Expansion of 1/((1-x^4)*(1-x^8)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence: number of partitions of n into parts 4, 8, 9 and 12. */
static seq_t *dp = NULL;          /* dynamic programming table: dp[i] = a(i) */
static idx_t dp_cap = 0;          /* allocated capacity of dp (>= n+1) */
static const idx_t parts[4] = {4, 8, 9, 12};

seq_t a(idx_t n)
{
    /* Ensure dp has enough space for indices 0..n */
    if (dp_cap < n + 1) {
        dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        dp_cap = n + 1;
    }

    /* Re‑initialize the table to zero, then set dp[0] = 1. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Classic coin‑change DP for partitions (order does not matter). */
    for (int k = 0; k < 4; ++k) {
        idx_t part = parts[k];
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    return dp[n];
}
