/*
 * A022261  Gaussian binomial coefficients [ n,10 ] for q = 9.
 * https://oeis.org/A022261
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product_{i=1..10} (9^{n-i+1}-1)/(9^i-1)
 *
 * Computed by dynamic programming:  [n,k]_q = [n-1,k]_q + 9^{n-k}*[n-1,k-1]_q
 * with k = 10 and base q = 9.
 *
 * This method keeps intermediate values small enough for 64‑bit arithmetic
 * while using arbitrary precision arithmetic for larger sequence builds.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Sequence offset is 10: values with n < 10 are not defined. */
    if (n < 10) return 0;

    const idx_t K = 10;          /* fixed k value */
    seq_t dp[K + 1];            /* DP array for Gaussian binomials */
    for (idx_t j = 0; j <= K; ++j)
        dp[j] = 0;
    dp[0] = 1;                  /* [n,0]_q = 1 for all n */

    seq_t base = (seq_t)9;       /* q */

    for (idx_t cur = 1; cur <= n; ++cur)
    {
        idx_t max_j = (cur < K) ? cur : K;
        for (idx_t j = max_j; j >= 1; --j)
        {
            idx_t exp = cur - j;
            seq_t pow_val = seq_pow(base, exp);            /* q^(cur-j) */
            seq_t prod = seq_mul(pow_val, dp[j - 1]);       /* q^(cur-j) * dp[j-1] */
            dp[j] = dp[j] + prod;                          /* dp[j] + prod */
        }
        /* dp[0] remains 1 */
    }

    return dp[K];
}
