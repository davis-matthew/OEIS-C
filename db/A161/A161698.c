/*
 * A161698  Number of reduced words of length n in the Weyl group B_5.
 * https://oeis.org/A161698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..5} (1 - x^{2k})/(1 - x)
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static seq_t coeff[26] = {0};
    static int initialized = 0;

    if (!initialized) {
        seq_t cur[26] = {0}, tmp[26];
        cur[0] = 1;
        idx_t cur_deg = 0;

        for (idx_t k = 1; k <= 5; ++k) {
            idx_t factor_len = 2 * k;
            idx_t new_deg = cur_deg + factor_len - 1;

            /* zero tmp array */
            for (idx_t i = 0; i <= new_deg; ++i)
                tmp[i] = 0;

            /* convolution of cur and a flat polynomial of length factor_len */
            for (idx_t i = 0; i <= cur_deg; ++i) {
                for (idx_t j = 0; j < factor_len; ++j) {
                    tmp[i + j] += cur[i];
                }
            }

            /* copy back to cur */
            cur_deg = new_deg;
            for (idx_t i = 0; i <= cur_deg; ++i)
                cur[i] = tmp[i];
        }

        /* store the final coefficients */
        for (idx_t i = 0; i <= cur_deg; ++i)
            coeff[i] = cur[i];

        initialized = 1;
    }

    return (n < 26) ? coeff[n] : 0;
}
