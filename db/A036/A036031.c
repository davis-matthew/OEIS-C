/*
 * A036031  Number of partitions of n into parts not of form 4k+2, 24k, 24k+5
 *          or 24k-5. Also number of partitions in which no odd part is
 *          repeated, with at most 2 parts of size less than or equal to 2 and
 *          where differences between parts at distance 5 are greater than 1
 *          when the smallest part is odd and great...
 * https://oeis.org/A036031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts that are NOT * of
 *            the form 4k+2, 24k, 24k+5, or 24k-5. Computed via an * ordinary
 *            partition DP
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* no terms below offset */

    static idx_t curMax = 0;
    static seq_t *dp = NULL;

    /* Allocate a larger table if required */
    if (n > curMax) {
        size_t newCap = (size_t)(n + 1);

        seq_t *newDp = NULL;
#ifdef __cplusplus
        newDp = new seq_t[newCap];
#else
        newDp = (seq_t*)malloc(newCap * sizeof(seq_t));
#endif
        if (!newDp) exit(1);               /* allocation failure */

        /* Initialise all entries to zero */
        for (size_t i = 0; i < newCap; ++i)
            newDp[i] = 0;
        newDp[0] = 1;                       /* empty partition */

        /* DP over allowed part sizes */
        for (idx_t m = 1; m <= n; ++m) {
            bool forbidden = false;
            if ((m % 4) == 2)          forbidden = true;
            else if (m % 24 == 0)      forbidden = true;
            else if (m % 24 == 5)      forbidden = true;
            else if (m % 24 == 19)     forbidden = true;

            if (forbidden) continue;

            for (idx_t i = m; i <= n; ++i)
                newDp[i] += newDp[i - m];
        }

        /* Replace the old table */
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        dp = newDp;
        curMax = n;
    }

    return dp[n];
}
