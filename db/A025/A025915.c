/*
 * A025915  Expansion of 1/((1-x^7)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(0)=1; for n>0, a(n)=a(n‑7)+a(n‑10)+a(n‑11) with missing indices treated as 0.
   The sequence counts partitions of n into parts 7, 10 and 11.  The implementation
   uses dynamic programming on the coin set {7,10,11}. */
seq_t a(idx_t n)
{
    /* Allocate space for DP table */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failed – unlikely for small n */

    /* initialise to zero; dp[0] will be set to 1 below */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* The three coin denominations */
    const idx_t coins[3] = {7, 10, 11};

    for (int c = 0; c < 3; ++c) {
        idx_t part = coins[c];
        if (part > n) continue;

        /* unbounded coin change DP update */
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
