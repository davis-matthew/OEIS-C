/*
 * A035393  Number of partitions of n into parts 7k or 7k+4.
 * https://oeis.org/A035393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 7k or 7k+4
 * Range:     u8 n <= 74, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n) {
    if (n == 0) return 1;            /* base case for DP */

    /* allocate DP table */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)(n + 1)]();    /* value‑initialized to 0 */
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    dp[0] = 1;                    /* empty partition */

    /* parts that are multiples of 7 */
    for (idx_t coin = 7; coin <= n; coin += 7) {
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    /* parts that are 4 mod 7 */
    for (idx_t coin = 4; coin <= n; coin += 7) {
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
