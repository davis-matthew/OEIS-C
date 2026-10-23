/*
 * A162165  Number of reduced words of length n in the Weyl group B_37.
 * https://oeis.org/A162165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{37} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_DEG 1405

seq_t a(idx_t n)
{
    /* The generating function for B_37 is
     * ∏_{k=1}^{37} (1 - x^{2k}) / (1 - x)
     * = ∏_{k=1}^{37} (1 + x + … + x^{2k-1})
     * whose degree is 1405, so coefficients beyond
     * n > 1405 are zero.
     */

    static bool initialized = false;
    static seq_t f[MAX_DEG + 1];
    static seq_t tmp[MAX_DEG + 1];

    if (!initialized) {
        /* initialise the coefficient array */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            f[i] = 0;
        f[0] = 1;

        idx_t deg = 0; /* current maximum degree */

        for (idx_t k = 1; k <= 37; ++k) {
            idx_t len   = 2 * k;                /* terms from 0..2k-1 */
            idx_t newdeg = deg + (len - 1);

            for (idx_t i = 0; i <= newdeg; ++i)
                tmp[i] = 0;

            for (idx_t i = 0; i <= deg; ++i) {
                seq_t coeff = f[i];
                if (coeff == 0) continue;
                for (idx_t j = 0; j < len; ++j) {
                    tmp[i + j] += coeff;
                }
            }

            /* copy results back */
            for (idx_t i = 0; i <= newdeg; ++i)
                f[i] = tmp[i];

            deg = newdeg;
        }
        initialized = true;
    }

    if (n <= MAX_DEG)
        return f[n];
    else
        return 0; /* coefficients beyond degree are zero */
}
