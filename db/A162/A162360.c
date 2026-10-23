/*
 * A162360  Number of reduced words of length n in the Weyl group D_21.
 * https://oeis.org/A162360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 75, u128 n <= 420,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The growth series for the Weyl group D_k is
 *   f(k) · ∏_{i=1}^{k-1} f(2i),
 * where f(m) = (1 - x^m)/(1 - x) = 1 + x + … + x^{m-1}.
 * This function returns the coefficient of x^n in that polynomial
 * for k = 21.  For k=21 the product is
 *   f(21) · ∏_{i=1}^{20} f(2i)
 * and the maximum degree is 420, so the coefficient array has size 421. */

#include <stddef.h>   /* for size_t, if needed */

#define MAX_COEFF 421

seq_t a(idx_t n)
{
    /* static coefficient array for the polynomial */
    static seq_t coeff[MAX_COEFF];
    static seq_t temp[MAX_COEFF];
    static int initialized = 0;

    if (!initialized) {
        /* zero the coefficient array */
        for (idx_t i = 0; i < MAX_COEFF; ++i) coeff[i] = 0;
        coeff[0] = 1;   /* start with polynomial 1 */

        idx_t old_len = 1; /* current number of coefficients */

        /* multiply by f(2), f(4), ..., f(40) */
        for (idx_t i = 1; i <= 20; ++i) {
            idx_t m = 2 * i;                    /* length of f(2i) is m */
            idx_t new_len = old_len + m - 1;     /* new number of terms */

            /* clear temp array up to new_len */
            for (idx_t k = 0; k < new_len; ++k) temp[k] = 0;

            for (idx_t a = 0; a < old_len; ++a) {
                for (idx_t b = 0; b < m; ++b) {
                    temp[a + b] += coeff[a];
                }
            }

            for (idx_t k = 0; k < new_len; ++k) coeff[k] = temp[k];
            old_len = new_len;
        }

        /* multiply by f(21) */
        idx_t m = 21;
        idx_t new_len = old_len + m - 1;

        for (idx_t k = 0; k < new_len; ++k) temp[k] = 0;

        for (idx_t a = 0; a < old_len; ++a) {
            for (idx_t b = 0; b < m; ++b) {
                temp[a + b] += coeff[a];
            }
        }

        for (idx_t k = 0; k < new_len; ++k) coeff[k] = temp[k];
        /* old_len is now MAX_COEFF */

        initialized = 1;
    }

    /* Return the requested coefficient, or 0 if the index is out of range. */
    if (n < MAX_COEFF) {
        return coeff[n];
    } else {
        return 0;
    }
}
