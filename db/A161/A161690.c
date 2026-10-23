/*
 * A161690  Number of reduced words of length n in the Weyl group A_45.
 * https://oeis.org/A161690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of \prod_{k=1..45}\,(1 + x + … + x^k) (OEIS
 *            A161690)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *coeff = NULL;   /* cached polynomial coefficients */
static idx_t  coeff_len = 0;  /* number of coefficients, 0 means not yet built */

seq_t a(idx_t n)
{
    /* Build the polynomial once, lazily. */
    if (coeff == NULL) {
        const idx_t M = 45;        /* Weyl group A_m with m = 45 */
        idx_t  len = 1;            /* current degree + 1 (initially 0 degrees) */

        /* Allocate initial polynomial P(x) = 1 */
        seq_t *poly = (seq_t *)malloc(sizeof(seq_t));
        poly[0] = (seq_t)1;

        for (idx_t k = 1; k <= M; ++k) {
            idx_t  prev_len  = len;
            idx_t  new_len   = prev_len + k;          /* new degree = prev + k */
            seq_t *newpoly   = (seq_t *)malloc(sizeof(seq_t) * new_len);

            /* convolution with (1 + x + … + x^k) using a sliding window */
            seq_t window = 0;
            for (idx_t i = 0; i < new_len; ++i) {
                if (i < prev_len)                 /* add next term of old poly */
                    window += poly[i];
                if (i > k) {                      /* remove term leaving the window */
                    idx_t out = i - k - 1;
                    if (out < prev_len)
                        window -= poly[out];
                }
                newpoly[i] = window;
            }

            free(poly);          /* replace old polynomial */
            poly = newpoly;
            len  = new_len;
        }

        coeff = poly;
        coeff_len = len;          /* degree 1035 + 1 = 1036 coefficients */
    }

    /* Return the requested coefficient, or 0 if out of bounds. */
    return (n < coeff_len) ? coeff[n] : (seq_t)0;
}
