/*
 * A161439  Number of reduced words of length n in the Weyl group A_7.
 * https://oeis.org/A161439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..7} (1 – x^{k+1})/(1–x) The
 *            generating function expands to the product of polynomials (1 + x
 *            + … + x^k) for k = 1…7. The resulting polynomial has degree 28,
 *            so a(n) = 0 for n > 28. The coefficients are computed once and
 *            cached
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Maximum possible length (degree of the polynomial). */
    const idx_t MAXLEN = 28;
    /* Static cache of coefficients. */
    static seq_t *coeff = NULL;

    /* Return zero for indices beyond the polynomial degree. */
    if (n > MAXLEN) {
        return (seq_t)0;
    }

    /* Lazy initialization of the coefficient array. */
    if (!coeff) {
        /* Allocate memory for MAXLEN + 1 coefficients. */
        coeff = (seq_t *)malloc((MAXLEN + 1) * sizeof(seq_t));
        if (!coeff) {
            /* If allocation fails, return zero (should not happen). */
            return (seq_t)0;
        }
        /* Initialise all coefficients to zero. */
        for (idx_t i = 0; i <= MAXLEN; ++i) {
            coeff[i] = (seq_t)0;
        }
        /* Start with the product-of-one factor (no factors yet). */
        coeff[0] = (seq_t)1;

        /* Sequentially convolve with each factor (1 + x + … + x^k). */
        for (idx_t k = 1; k <= 7; ++k) {
            /* Temporary array for the new coefficients. */
            seq_t tmp[MAXLEN + 1];
            for (idx_t i = 0; i <= MAXLEN; ++i) {
                tmp[i] = (seq_t)0;
            }
            for (idx_t i = 0; i <= MAXLEN; ++i) {
                if (coeff[i] == (seq_t)0) {
                    continue;   /* Skip zero terms for speed. */
                }
                for (idx_t j = 0; j <= k; ++j) {
                    if (i + j > MAXLEN) {
                        break;
                    }
                    tmp[i + j] = tmp[i + j] + coeff[i];
                }
            }
            /* Copy the temporary results back to the coefficient cache. */
            for (idx_t i = 0; i <= MAXLEN; ++i) {
                coeff[i] = tmp[i];
            }
        }
    }
    /* Return the cached coefficient. */
    return coeff[n];
}
