/*
 * A162377  Number of reduced words of length n in the Weyl group D_30.
 * https://oeis.org/A162377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in f(30)*Π_{i=1}^{29} f(2i), where
 *            f(m) = 1 + x + … + x^{m-1} (the growth series of D_30)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 40, u128 n <= 353,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precomputed polynomial coefficients (degree 870). */
    static seq_t coeff[871];
    static bool built = false;

    if (!built) {
        /* Initialize the coefficient array. */
        for (idx_t i = 0; i < 871; ++i) coeff[i] = 0;
        coeff[0] = 1;                /* f(30) starts as 1. */
        idx_t cur_len = 1;           /* number of coefficients computed so far. */

        /* The list of m values for the factors f(m). */
        const idx_t ms[30] = {
            30,
            2,4,6,8,10,12,14,16,18,20,
            22,24,26,28,30,32,34,36,38,40,
            42,44,46,48,50,52,54,56,58
        };

        for (idx_t k = 0; k < 30; ++k) {
            idx_t m = ms[k];
            idx_t new_len = cur_len + m - 1;

            static seq_t temp[871];
            for (idx_t i = 0; i < new_len; ++i) temp[i] = 0;

            /* Convolution: (1 + x + … + x^{m-1}) * current_poly. */
            for (idx_t i = 0; i < cur_len; ++i) {
                seq_t val = coeff[i];
                for (idx_t j = 0; j < m; ++j)
                    temp[i + j] += val;
            }

            for (idx_t i = 0; i < new_len; ++i) coeff[i] = temp[i];
            cur_len = new_len;
        }

        built = true;
    }

    if (n < 871) return coeff[n];
    return (seq_t)0;   /* coefficients beyond degree 870 are zero. */
}
