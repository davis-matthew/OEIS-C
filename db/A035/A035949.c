/*
 * A035949  Number of partitions in parts not of the form 13k, 13k+1 or 13k-1.
 *          Also number of partitions with no part of size 1 and differences
 *          between parts at distance 5 are greater than 1.
 * https://oeis.org/A035949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            1 or 12 modulo 13 (i.e., parts with size ≡ 2‥11 (mod 13))
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for any n ≥ 1.  Uses an O(n²) DP over the allowed part sizes. */
seq_t a(idx_t n)
{
    /* Allocate temporary array of size n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure – unlikely */

    /* initialise dp array to 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                   /* empty partition */

    /* iterate over allowed part sizes */
    for (idx_t part = 2; part <= n; ++part)
    {
        idx_t r = part % 13;
        if (r == 0 || r == 1 || r == 12)
            continue;            /* forbidden residue */

        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] += dp[sum - part];
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
