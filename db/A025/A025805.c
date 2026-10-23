/*
 * A025805  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^11)).
 * https://oeis.org/A025805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 4, and 11
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming: dp[0] = 1; for each part p,
   for j = p .. n  dp[j] += dp[j - p]; */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The allowed parts */
    static const idx_t parts[3] = { 2, 4, 11 };

    /* Allocate and zero‑initialize the DP table */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp)  /* allocation failure → return 0 */
        return 0;

    dp[0] = 1;  /* one way to make sum 0 */

    /* Update counts for each allowed part */
    for (idx_t k = 0; k < 3; ++k) {
        idx_t p = parts[k];
        if (p > n) continue;
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
