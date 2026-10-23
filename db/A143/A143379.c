/*
 * A143379  Expansion of q^(-7/24) * eta(q) * eta(q^4)^2 / eta(q^2) in powers
 *          of q.
 * https://oeis.org/A143379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in product_{k>0} (1 - x^(4*k))^2 * (1
 *            - x^(2*k-1))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate coefficient array */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;     /* out of memory: return 0 (unlikely) */
    /* initialize */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    const seq_t NEG1 = (seq_t)-1;
    const seq_t NEG2 = (seq_t)-2;

    for (idx_t m = 1; m <= n; ++m) {
        if (m % 4 == 0) {                 /* factor (1 - 2x^m + x^(2m)) */
            for (idx_t k = n; k >= m; --k) {
                dp[k] += dp[k - m] * NEG2;        /* -2*x^m term */
                if (k >= 2 * m) {
                    dp[k] += dp[k - 2 * m];        /* +1*x^(2m) term */
                }
            }
        } else if (m % 2 == 1) {           /* factor (1 - x^m)   */
            for (idx_t k = n; k >= m; --k) {
                dp[k] += dp[k - m] * NEG1;
            }
        }
        /* even m not divisible by 4 contribute nothing */
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
