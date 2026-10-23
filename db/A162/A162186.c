/*
 * A162186  Number of reduced words of length n in the Weyl group B_46.
 * https://oeis.org/A162186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = [x^n] ∏_{k=1}^{46} (1-x^(2k))/(1-x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 26, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162186: Number of reduced words of length n in the Weyl group B_46 */
/*          = coefficient of x^n in the polynomial ∏_{k=1}^{46} (1 + x + … + x^{2k-1}) */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* m = 46, maximum degree = m^2 = 2116 */
    static const int MAX_DEG = 46 * 46;

    /* Lazy static allocation of coefficient arrays */
    static seq_t *coeff = NULL;
    static seq_t *temp  = NULL;
    static int    initialized = 0;  /* flag to indicate first call */

    if (!initialized) {
        /* Allocate space for the final coefficient vector (degree 0..2116) */
        coeff = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
        temp  = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
        if (!coeff || !temp) {
            /* Out of memory – return zero on failure (should never happen in tests). */
            return (seq_t)0;
        }

        /* Initialise first polynomial = 1 */
        memset(coeff, 0, (MAX_DEG + 1) * sizeof(seq_t));
        coeff[0] = (seq_t)1;
        idx_t old_len = 1;          /* current length (degree + 1) of coeff */

        /* Multiply successively by (1 + x + … + x^{2k-1}) for k = 1 … 46 */
        for (int k = 1; k <= 46; ++k) {
            int L = 2 * k;                     /* length of the next factor */
            idx_t new_len = old_len + L - 1;    /* new length after convolution */

            /* Reset temp array for the new convolution */
            memset(temp, 0, new_len * sizeof(seq_t));

            /* Convolution: add coeff[i] shifted by 0 .. L-1 */
            for (idx_t i = 0; i < old_len; ++i) {
                seq_t val = coeff[i];
                if (val == (seq_t)0) continue;
                for (int j = 0; j < L; ++j) {
                    temp[i + j] += val;
                }
            }

            /* Copy the result back into coeff */
            memcpy(coeff, temp, new_len * sizeof(seq_t));

            old_len = new_len;
        }

        initialized = 1;  /* mark as done */
    }

    if (n <= MAX_DEG) {
        return coeff[n];
    } else {
        return (seq_t)0;  /* coefficients beyond degree are zero */
    }
}
