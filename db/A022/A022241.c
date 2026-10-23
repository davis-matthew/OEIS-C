/*
 * A022241  Gaussian binomial coefficients [ n,12 ] for q = 7.
 * https://oeis.org/A022241
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..12} (7^(n-i+1)-1)/(7^i-1) (Gaussian binomial
 *            [n,12]_7)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 12)
        return 0; /* values before the offset are irrelevant */

    const int K = 12;
    seq_t dp[K + 1];

    dp[0] = 1;
    for (int j = 1; j <= K; ++j)
        dp[j] = 0;

    for (idx_t m = 1; m <= n; ++m) {
        int maxj = (m < K) ? (int)m : K;
        seq_t power = big_pow((seq_t)7, (seq_t)(m - maxj));

        for (int j = maxj; j >= 1; --j) {
            seq_t term = power * dp[j - 1];
            dp[j] += term;
            power *= (seq_t)7;
        }
    }

    return dp[K];
}
