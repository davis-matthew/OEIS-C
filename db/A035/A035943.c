/*
 * A035943  Number of partitions of n into parts not of the form 9k, 9k+4 or
 *          9k-4. Also number of partitions with at most 3 parts of size 1 and
 *          differences between parts at distance 3 are greater than 1.
 * https://oeis.org/A035943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts not of the form 9k, 9k+4 or
 *            9k-4
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 175, u64 n <= 588,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer to store results of the dynamic programming */
    static seq_t *dp = NULL;
    static idx_t dp_max = 0;

    /* Grow or shrink the array if needed */
    if (dp == NULL || n > dp_max) {
        if (n > dp_max) dp_max = n;
        seq_t *tmp = (seq_t*)realloc(dp, (dp_max + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* out of memory – caller will not crash */
        dp = tmp;
    }

    /* initialise the DP array */
    for (idx_t i = 0; i <= dp_max; ++i) dp[i] = 0;
    dp[0] = 1;

    /* dynamic programming : coin‑change style */
    for (idx_t p = 1; p <= dp_max; ++p) {
        idx_t r = p % 9;
        if (r == 0 || r == 4 || r == 5) continue;   /* forbidden residues */
        for (idx_t m = p; m <= dp_max; ++m) {
            dp[m] += dp[m - p];
        }
    }

    return dp[n];
}
