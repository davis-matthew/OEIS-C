/*
 * A162210  Number of reduced words of length n in the Weyl group D_7.
 * https://oeis.org/A162210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in the product (1 + x + … + x^6) · (1
 *            + x) · (1 + x + x^2 + x^3) · (1 + x + … + x^5) · (1 + x + … +
 *            x^7) · (1 + x + … + x^9) · (1 + x + … + x^11)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    /* The polynomial has degree 42, coefficients 0 … 42 */
    static seq_t coeff[43];

    if (!init) {
        static const idx_t factors[7] = {7, 2, 4, 6, 8, 10, 12};
        seq_t dp[43] = {0};
        seq_t newdp[43];
        dp[0] = 1;              /* initial polynomial = 1 */
        idx_t current = 0;      /* current maximum degree */

        for (int f = 0; f < 7; ++f) {
            idx_t a = factors[f];
            /* zero out newdp */
            for (idx_t i = 0; i < 43; ++i)
                newdp[i] = 0;

            /* convolution: multiply by (1 + x + … + x^(a-1)) */
            for (idx_t i = 0; i <= current; ++i) {
                seq_t val = dp[i];
                if (val == 0) continue;
                for (idx_t k = 0; k < a; ++k)
                    newdp[i + k] += val;
            }
            current += a - 1;
            /* copy newdp back to dp */
            for (idx_t i = 0; i <= current; ++i)
                dp[i] = newdp[i];
        }
        /* store the coefficients */
        for (idx_t i = 0; i <= current; ++i)
            coeff[i] = dp[i];
        init = true;
    }

    if (n <= 42)
        return coeff[n];
    return (seq_t)0;           /* degree > 42 -> coefficient is zero */
}
