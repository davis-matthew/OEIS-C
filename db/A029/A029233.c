/*
 * A029233  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 7, 10, 11
 * Range:     u8 n <= 118, u16 n <= 831, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* If we have no cached array yet, or the requested n exceeds the
       currently computed range, recompute up to n from scratch. */
    if (!dp || n > max_n) {
        /* Allocate a new array of length n+1.  Use malloc; the
           required cast is legal for C++ as well. */
        seq_t *new_dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;           /* out of memory – graceful failure */

        /* Initialise all entries to zero */
        for (idx_t i = 0; i <= n; ++i) new_dp[i] = (seq_t)0;
        /* The empty partition counts as one way for n = 0 */
        new_dp[0] = (seq_t)1;

        /* The part sizes allowed in this sequence */
        const idx_t coins[4] = {(idx_t)2, (idx_t)7, (idx_t)10, (idx_t)11};

        /* Standard coin‑change DP: for each coin, add the ways to
           reach each amount that includes that coin. */
        for (idx_t c = 0; c < 4; ++c) {
            idx_t coin = coins[c];
            for (idx_t i = coin; i <= n; ++i) {
                new_dp[i] += new_dp[i - coin];
            }
        }

        /* Free the old cached array if it existed, and replace it. */
        if (dp) free(dp);
        dp = new_dp;
        max_n = n;
    }

    /* Return the pre‑computed value for the requested n. */
    return dp[n];
}
