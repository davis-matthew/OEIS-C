/*
 * A014126  Number of partitions of 2*n into at most 4 parts.
 * https://oeis.org/A014126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 2*n into at most 4 parts
 *            (equivalently, number of partitions of 2*n into parts of size
 *            ≤4)
 * Range:     u8 n <= 14, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t precomp_max = (idx_t)-1;  /* invalid initial value */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    idx_t m = 2 * n;                     /* target integer */

    /* Allocate or resize the DP table if necessary.  We recompute
       the entire table each time m increases to avoid double counting. */
    if (dp == NULL || m > precomp_max) {
        seq_t *newdp = (seq_t *)realloc(dp, (size_t)(m + 1) * sizeof(seq_t));
        if (!newdp) return (seq_t)0;     /* out‑of‑memory fallback */
        dp = newdp;

        /* reset all entries to 0 and set base case */
        for (idx_t i = 0; i <= m; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* standard coin‑change DP: unlimited coins of sizes 1…4 */
        for (idx_t coin = 1; coin <= 4; ++coin)
            for (idx_t i = coin; i <= m; ++i)
                dp[i] += dp[i - coin];

        precomp_max = m;
    }

    return dp[m];
}
