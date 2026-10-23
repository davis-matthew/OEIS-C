/*
 * A035981  Number of partitions in parts not of the form 21k, 21k+3 or 21k-3.
 *          Also number of partitions with at most 2 parts of size 1 and
 *          differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A035981: number of partitions of n using parts not
   congruent to 0, 3 or 18 modulo 21.  The implementation uses a
   standard dynamic programme over allowed part sizes. */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[N + 1];
    dp[0] = 1;
    for (size_t i = 1; i <= N; ++i) dp[i] = 0;
#else
    dp = calloc(N + 1, sizeof(seq_t));
    if (!dp) return 0;      /* allocation failure */
    dp[0] = 1;
#endif

    for (size_t part = 1; part <= N; ++part) {
        int r = part % 21;
        if (r == 0 || r == 3 || r == 18) continue;   /* forbidden part size */
        for (size_t sum = part; sum <= N; ++sum) {
            dp[sum] = dp[sum] + dp[sum - part];
        }
    }

    seq_t result = dp[N];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
