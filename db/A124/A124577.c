/*
 * A124577  Define p(alpha) to be the number of H-conjugacy classes where H is
 *          a Young subgroup of type alpha of the symmetric group S_n. Then
 *          a(n) = sum p(alpha) where |alpha| = n and alpha has at most n
 *          parts.
 * https://oeis.org/A124577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} p(n,k) * n^k, where p(n,k) is the number of
 *            partitions of n with largest part k. Equivalently, a(n) is the
 *            coefficient of x^n in ∏_{k≥1} 1/(1 - n x^k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Allocate DP array of size n+1 */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;      /* memory error – not expected */

    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    /* Dynamic programming:
       For each part size k (1..n), update all j (k..n)
       dp[j] += n * dp[j - k]  (multiplication by small integer n). */
    unsigned int mult = (unsigned int)n;
    for (size_t k = 1; k <= N; ++k) {
        for (size_t j = k; j <= N; ++j) {
            dp[j] = dp[j] + mul_small(dp[j - k], mult);
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
