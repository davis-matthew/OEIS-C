/*
 * A029254  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A029254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 643, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the partitions of n into the parts 3, 4, 6, and 10
   using a standard coin‑change dynamic programming approach.   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Array that stores the number of partitions for each
       target amount up to the current argument n.  It is
       allocated on the first call and re‑allocated whenever
       a larger n is requested.  The array is then recomputed
       from scratch on every call, which is fast enough for
       n <= 10000.                                                          */
    static seq_t *dp = NULL;
    static idx_t dp_len = 0;

    if (n + 1 > dp_len) {
        /* Grow the array to fit 0 .. n inclusive. */
        dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        dp_len = n + 1;
    }

    /* Initialise the DP table.  Only dp[0] is initially 1. */
    seq_t zero = 0;
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = zero;
    dp[0] = 1;

    /* The allowed parts: 3, 4, 6, 10.  For each part we
       update the DP table cumulatively. */
    const idx_t parts[4] = {3, 4, 6, 10};
    for (int k = 0; k < 4; ++k) {
        idx_t part = parts[k];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    return dp[n];
}
