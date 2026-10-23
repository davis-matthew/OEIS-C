/*
 * A035465  Number of partitions of n into parts 8k+4 or 8k+7.
 * https://oeis.org/A035465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 4 or 7
 *            mod 8
 * Range:     u8 n <= 85, u16 n <= 200, u32 n <= 553, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate array for dynamic programming */
    seq_t *dp;
    size_t N = (size_t)n + 1;
#if defined(__cplusplus)
    dp = new seq_t[N]();
#else
    dp = (seq_t*)calloc(N, sizeof(seq_t));
#endif

    dp[0] = 1;

    /* Add parts of the form 8k+4 */
    for (idx_t p = 4; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    /* Add parts of the form 8k+7 */
    for (idx_t p = 7; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
