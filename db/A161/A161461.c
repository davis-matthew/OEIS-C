/*
 * A161461  Number of reduced words of length n in the Weyl group A_12.
 * https://oeis.org/A161461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the coefficient of x^n in * Π_{k=1}^{12} (1 + x + ⋯ +
 *            x^k) * (same as the number of reduced words of length n in the
 *            Weyl group A₁₂)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_SUM 78  // 12*13/2

seq_t a(idx_t n)
{
    static bool done = false;
    static seq_t coeff[MAX_SUM + 1] = {0};

    if (!done) {
        /* DP arrays – constant size 79 */
        seq_t dp[MAX_SUM + 1]   = {0};
        seq_t ndp[MAX_SUM + 1]  = {0};

        dp[0] = 1;  // base case

        /* iterate over the 12 factors */
        for (idx_t k = 1; k <= 12; ++k) {
            idx_t max = k * (k + 1) / 2;  /* highest degree after this factor */

            seq_t sum = 0;
            for (idx_t i = 0; i <= max; ++i) {
                sum += dp[i];
                if (i > k) sum -= dp[i - k - 1];
                ndp[i] = sum;
            }
            /* copy new values back to dp for the next iteration */
            for (idx_t i = 0; i <= max; ++i)
                dp[i] = ndp[i];
        }

        /* store the completed coefficients */
        for (idx_t i = 0; i <= MAX_SUM; ++i)
            coeff[i] = dp[i];

        done = true;
    }

    if (n > MAX_SUM)
        return 0;          /* coefficients beyond degree 78 are zero */
    return coeff[n];
}
