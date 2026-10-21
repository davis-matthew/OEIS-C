/*
 * A210632  G.f. for Ehrhart quasi-polynomials for hyperplane arrangements of
 *          type E_8.
 * https://oeis.org/A210632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in x^30 / ((x-1)^10 * (x+1)^6 *
 *            (x^2-x+1) * (x^2+1)^2 * (x^2+x+1)^3 * (x^4+x^3+x^2+x+1))
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* the first non‑zero term occurs at index 30 */
    if (n < 30) return 0;

    /* we need coefficients of
     * 1/((1-x)^1 (1-x^2)^3 (1-x^3)^2 (1-x^4)^2 (1-x^5)^1 (1-x^6)^1 )
     * shifted by 30.  This is the same as convolving
     * ten copies of 1/(1-x^k) for k = 1,2,2,2,3,3,4,4,5,6.
     */
    idx_t m = n - 30;               /* maximum exponent to compute */
    seq_t *dp      = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= m; ++i) dp[i] = 0;
    dp[0] = 1;                      /* coefficient of x^0 is 1 */

    const int factors[10] = {1, 2, 2, 2, 3, 3, 4, 4, 5, 6};

    for (int f = 0; f < 10; ++f) {
        int k = factors[f];
        /* convolution with 1/(1-x^k) using the recurrence
         *   dp_next[i] = dp[i] + dp_next[i-k]   (i≥k)
         *   dp_next[i] = dp[i]                  (i<k)
         */
        for (idx_t i = 0; i <= m; ++i) {
            if (i >= (idx_t)k)
                dp_next[i] = dp[i] + dp_next[i - k];
            else
                dp_next[i] = dp[i];
        }
        /* swap dp and dp_next for the next factor */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp[m];
    free(dp);
    free(dp_next);
    return result;
}
