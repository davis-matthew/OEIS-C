/*
 * A109699  Number of partitions of n into parts each equal to 3 mod 5.
 * https://oeis.org/A109699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts each ≡ 3 (mod 5)
 * Range:     u8 n <= 94, u16 n <= 230, u32 n <= 654, u64 n <= 2090,
 *            u128 n <= 7268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1];
#else
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!dp) return 0; /* allocation failed */

    /* Initialise to 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Coin-change DP over parts 3,8,13,… (=3 mod 5) */
    for (idx_t part = 3; part <= n; part += 5) {
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
