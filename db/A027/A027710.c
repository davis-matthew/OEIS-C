/*
 * A027710  Number of ways of placing n labeled balls into n unlabeled (but
 *          3-colored) boxes.
 * https://oeis.org/A027710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^n 3^k * S(n,k) (Stirling numbers of the second
 *            kind) Equivalent e.g.f.: exp(3*(e^x-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  // memory allocation failed

    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    seq_t sum = 0;

    for (size_t cur = 1; cur <= N; ++cur) {
        for (size_t k = cur; k > 0; --k) {
            seq_t old_val = dp[k];
            seq_t new_val = (seq_t)k * old_val + (seq_t)3 * dp[k - 1];
            sum += new_val - old_val;
            dp[k] = new_val;
        }
        dp[0] = 0;  /* dp[0] is zero for n ≥ 1 */
    }

    free(dp);
    return sum;
}
