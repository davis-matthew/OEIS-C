/*
 * A036019  Number of partitions of n into parts not of form 4k+2, 12k, 12k+5
 *          or 12k-5.
 * https://oeis.org/A036019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p <= n, p allowed} a(n-p), a(0)=1 * Partitions into
 *            parts not of form 4k+2, 12k, 12k+5, or 12k-5. * Allowed residues
 *            mod 12 are 1,3,4,8,9,11
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate DP array */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* fallback on allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                   /* base case */

    for (idx_t p = 1; p <= n; ++p) {
        idx_t mod = p % 12;
        if (mod == 1 || mod == 3 || mod == 4 ||
            mod == 8 || mod == 9 || mod == 11) {
            for (idx_t i = p; i <= n; ++i)
                dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
