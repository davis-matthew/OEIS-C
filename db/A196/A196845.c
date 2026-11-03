/*
 * A196845  Table of elementary symmetric function a_k(3,4,...,n+2) (no 1 and
 *          2).
 * https://oeis.org/A196845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the coefficient a_k of x^k in * ∏_{i=3}^{r+2} (1 + i x)
 *            , * where r and k are obtained from the linear index n: * r =
 *            floor( (sqrt(8 n+1) – 1) / 2 ), * k = n – r(r+1)/2 . * The
 *            sequence starts 1,1,3,1,7,12,1,
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the elementary symmetric sum of degree k of {3,…,r+2}. */
static seq_t elementary_symmetric(idx_t r, idx_t k)
{
    /* dp[j] holds a_j after processing some elements. */
    size_t K = (size_t)k;

#if defined(__cplusplus)
    seq_t *dp = new seq_t[K + 1];
#else
    seq_t *dp = (seq_t *)malloc((K + 1) * sizeof(seq_t));
#endif
    if (!dp) return (seq_t)0;

    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= K; ++i) dp[i] = (seq_t)0;

    for (idx_t v = 3; v <= r + 2; ++v) {
        size_t maxj = (v - 2 < (idx_t)K) ? (v - 2) : K;
        for (size_t j = maxj; j >= 1; --j) {
            dp[j] = dp[j] + dp[j - 1] * (seq_t)v;
        }
    }

    seq_t result = dp[K];
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}

seq_t a(idx_t n)
{
    /* Determine r so that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t k = n - r * (r + 1) / 2;
    if (k == 0) return (seq_t)1;          /* a_0 = 1 */

    return elementary_symmetric(r, k);
}
