/*
 * A162679  G.f. is the polynomial (Product_{k=1..22} (1 - x^(3*k)))/(1-x)^22.
 * https://oeis.org/A162679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in product_{k=1..22} (1 + x + … +
 *            x^{3k-1})
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 64, u128 n <= 737,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The generating function is (∏_{k=1}^{22}(1−x^{3k}))/(1−x)^{22} ,
   which reduces to the polynomial above.  All terms beyond
   degree 737 are zero.  */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum index with a non‑zero value is 737 */
    static seq_t coeff[738];
    static int ready = 0;

    if (!ready) {
        /* initialise to zero */
        for (size_t i = 0; i < 738; ++i)
            coeff[i] = 0;
        coeff[0] = 1;
        size_t curr_len = 1;
        seq_t temp[738];

        for (idx_t k = 1; k <= 22; ++k) {
            size_t m = 3 * k;                     /* number of terms in factor */
            size_t new_len = curr_len + m - 1;     /* length after convolution */

            /* zero temporary buffer */
            for (size_t i = 0; i < new_len; ++i)
                temp[i] = 0;

            /* convolution with an all‑ones polynomial of length m */
            for (size_t i = 0; i < curr_len; ++i) {
                seq_t v = coeff[i];
                for (size_t t = 0; t < m; ++t)
                    temp[i + t] += v;
            }

            /* copy result back into coeff and update length */
            for (size_t i = 0; i < new_len; ++i)
                coeff[i] = temp[i];
            curr_len = new_len;
        }
        ready = 1;
    }

    if (n >= 738)
        return 0;   /* coefficients beyond degree 737 are zero */

    return coeff[(size_t)n];
}
