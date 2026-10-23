/*
 * A143277  G.f.: Product_{k>0} (1 - x^(3*k))^2 / (1 - x^(3*k - 2)).
 * https://oeis.org/A143277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{partitions of n with parts 1,4,7,10,…} (−1)^(#parts of
 *            size 3k) · (1) etc. * Computation via dynamic programming: * -
 *            First apply the denominator product (1−x^{3k−2})⁻¹ to generate
 *            partition counts * - Then apply each numerator factor
 *            (1−2x^{3k}+x^{6k}) to obtain the final coefficients...
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* Persistent DP array reused across calls */
    static seq_t *dp = NULL;
    static idx_t cap = 0;

    /* Ensure enough space */
    if (dp == NULL || cap <= n) {
        idx_t newcap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        dp = tmp;
        cap = newcap;
    }

    /* Initialise dp[0] = 1, others = 0 up to n */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Denominator: product_{k>0} (1 - x^{3k-2})^{-1}
     *   parts allowed: 1,4,7,10,...
     */
    for (idx_t m = 1; m <= n; m += 3) {
        for (idx_t i = m; i <= n; ++i)
            dp[i] = dp[i] + dp[i - m];
    }

    /* Numerator: product_{k>0} (1 - 2x^{3k} + x^{6k})
     *   For each k, update dp in reverse to avoid overwriting needed values
     */
    for (idx_t m = 3; m <= n; m += 3) {
        for (idx_t i = n; i >= m; --i) {
            seq_t a_val = dp[i];
            seq_t b_val = dp[i - m];
            seq_t c_val = (i >= 2 * m) ? dp[i - 2 * m] : 0;
            dp[i] = a_val - 2 * b_val + c_val;
        }
    }

    return dp[n];
}
