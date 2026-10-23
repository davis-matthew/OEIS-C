/*
 * A029159  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 7 and 11.
 *            Computed by a simple dynamic–programming routine
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate or grow a static DP array the first time it is needed. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    if (n + 1 > dp_size) {
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!new_dp) return 0;          /* allocation failure – unlikely */
        dp = new_dp;
        dp_size = n + 1;
    }

    /* Initialize the array up to n. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* The allowed parts. */
    const idx_t parts[4] = {2, 3, 7, 11};

    /* Standard unbounded‑knapsack DP: for each part, add ways to form larger sums. */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    return dp[n];
}
