/*
 * A162297  Number of reduced words of length n in the Weyl group D_12.
 * https://oeis.org/A162297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the coefficient of x^n in the polynomial * f(12) *
 *            Prod_{i=1..11} f(2*i), where f(m) = 1 + x + … + x^(m-1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 39, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The degree of the product is 11 + (1+3+…+21) = 132. */
    static seq_t coeff[133] = {0};
    static int ready = 0;

    if (!ready) {
        /* Start with f(12) = 1 + x + … + x^11. */
        for (size_t i = 0; i < 12; ++i)
            coeff[i] = 1;
        size_t max_deg = 11;               /* current maximum exponent */

        /* Multiply successively by f(2*i) for i = 1..11. */
        for (idx_t i = 1; i <= 11; ++i) {
            size_t m = 2 * i;              /* length of f(2*i) = 2*i */
            seq_t tmp[133] = {0};           /* temporary array for convolution */

            /* Convolution: coeff * [1,1,…,1] (length m). */
            for (size_t d = 0; d <= max_deg; ++d) {
                if (coeff[d] == 0)
                    continue;
                for (size_t k = 0; k < m; ++k)
                    tmp[d + k] += coeff[d];
            }

            /* Update the degree and copy results back. */
            max_deg += m - 1;
            for (size_t d = 0; d <= max_deg; ++d)
                coeff[d] = tmp[d];
        }
        ready = 1;
    }

    return (n <= 132) ? coeff[n] : 0;
}
