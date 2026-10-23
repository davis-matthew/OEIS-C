/*
 * A162646  G.f. is the polynomial (Product_{k=1..21} (1 - x^(3*k)))/(1-x)^21.
 * https://oeis.org/A162646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∏_{k=1}^{21} (1 -
 *            x^{3k}))/(1-x)^21. After canceling the factor (1-x)^21, this
 *            equals the coefficient of x^n in ∏_{k=1}^{21} (1 + x + … +
 *            x^{3k-1}). Hence it is a polynomial of degree 672 with 673
 *            non‑zero terms. The implementation precomputes all coefficients
 *            once o...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 70, u128 n <= 672,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* the maximum possible degree is 672; allocate one more for safety */
    enum { MAX_DEGREE = 672, LEN = MAX_DEGREE + 1 };

    /* one‑time precomputation of the polynomial coefficients */
    static seq_t coeff[LEN];
    static idx_t cached_deg = 0;
    static int computed = 0;

    if (!computed) {
        /* initialise all coefficients to zero */
        for (idx_t i = 0; i < LEN; ++i)
            coeff[i] = 0;
        coeff[0] = 1;           /* start with polynomial = 1 */
        cached_deg = 0;

        /* multiply by (1 + x + … + x^{3k-1}) for k = 1 .. 21 */
        for (idx_t k = 1; k <= 21; ++k) {
            idx_t len = 3 * k;                  /* number of terms in this factor */
            idx_t new_deg = cached_deg + (len - 1);

            /* temporary array for the new coefficients */
            seq_t tmp[LEN];
            for (idx_t i = 0; i < LEN; ++i)
                tmp[i] = 0;

            /* convolution: multiply current polynomial by the block of ones */
            for (idx_t i = 0; i <= cached_deg; ++i) {
                seq_t val = coeff[i];
                if (val == 0)
                    continue;
                for (idx_t j = 0; j < len; ++j)
                    tmp[i + j] += val;
            }

            /* copy result back to coeff array and update degree */
            for (idx_t i = 0; i <= new_deg; ++i)
                coeff[i] = tmp[i];
            cached_deg = new_deg;
        }
        computed = 1;
    }

    /* coefficients beyond the computed degree are zero */
    if (n > cached_deg)
        return 0;
    return coeff[n];
}
