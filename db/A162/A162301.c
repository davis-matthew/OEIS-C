/*
 * A162301  Number of reduced words of length n in the Weyl group D_14.
 * https://oeis.org/A162301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in the product f(14) * ∏_{i=1}^{13}
 *            f(2i), where f(m) = (1-x^m)/(1-x) = 1 + x + … + x^{m-1}. The
 *            polynomial has degree 182; all other coefficients are 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 182, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Max degree of the product polynomial */
    const idx_t MAX_DEG = 182;
    static seq_t coeff[183];
    static seq_t tmp[183];
    static bool initialized = false;

    if (!initialized) {
        /* initialize coefficient array to zero except coeff[0] = 1 */
        for (size_t i = 0; i <= (size_t)MAX_DEG; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        /* list of m-values: 14 and 2,4,6,…,26 (13 terms) */
        const idx_t mvals[] = {14, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26};
        const size_t num_m = sizeof mvals / sizeof mvals[0];

        for (size_t k = 0; k < num_m; ++k) {
            idx_t m = mvals[k];
            /* clear temporary array */
            for (size_t i = 0; i <= (size_t)MAX_DEG; ++i)
                tmp[i] = 0;
            for (size_t i = 0; i <= (size_t)MAX_DEG; ++i) {
                seq_t val = coeff[i];
                if (val == 0) continue;
                for (size_t j = 0; j < (size_t)m; ++j) {
                    if (i + j > (size_t)MAX_DEG) break;
                    tmp[i + j] += val;
                }
            }
            /* copy result back to coeff */
            for (size_t i = 0; i <= (size_t)MAX_DEG; ++i)
                coeff[i] = tmp[i];
        }
        initialized = true;
    }

    if (n > MAX_DEG)
        return 0;
    return coeff[(size_t)n];
}
