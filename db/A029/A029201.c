/*
 * A029201  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts {2,5,7,10}
 * Range:     u8 n <= 90, u16 n <= 638, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;          /* allocation failure – return 0 */

    dp[0] = (seq_t)1;             /* one way to partition 0 */

    /* Parts allowed in the partitions */
    static const idx_t parts[4] = { 2, 5, 7, 10 };

    /* DP: for each part size, add its contribution to all reachable indices */
    for (int k = 0; k < 4; ++k) {
        idx_t coin = parts[k];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    /* Result before freeing the array */
    seq_t res = dp[n];
    free(dp);
    return res;
}
