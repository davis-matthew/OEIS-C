/*
 * A162389  Number of reduced words of length n in the Weyl group D_37.
 * https://oeis.org/A162389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = coefficient of x^n in (1 - x^m) * ∏_{i=1}^{m-1} (1 -
 *            x^{2i}) / (1 - x)^m For m = 37 this is the generating function
 *            for the number of reduced words of length n in the Weyl group
 *            D_37. The polynomial has degree m(m-1) = 1332
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const idx_t M = 37;
    static const idx_t DEG = M * (M - 1);            /* maximum degree = 1332 */

    /* Cache of polynomial coefficients, computed once */
    static seq_t *coeff = NULL;

    if (coeff == NULL) {
        size_t sz = (size_t)DEG + 1;
        coeff = (seq_t *)malloc(sz * sizeof(seq_t));
        if (coeff == NULL) return 0;   /* allocation failure */

        /* Initialise polynomial to 1 */
        for (idx_t i = 0; i <= DEG; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* Temporary array for convolution */
        seq_t *tmp = (seq_t *)malloc(sz * sizeof(seq_t));
        if (tmp == NULL) { free(coeff); coeff = NULL; return 0; }

        idx_t cur_deg = 0;                     /* degree of current polynomial */

        /* There are M factors: first factor length = M,
           subsequent factors length = 2*fi for fi = 1 .. M-1 */
        for (idx_t fi = 0; fi < M; ++fi) {
            idx_t L = (fi == 0) ? M : (2 * fi);  /* length of the (fi+1)-th factor */
            idx_t new_deg = cur_deg + (L - 1);
            if (new_deg > DEG) new_deg = DEG;

            /* zero relevant part of tmp */
            for (idx_t i = 0; i <= new_deg; ++i) tmp[i] = 0;

            /* convolution of current coeff with 1's of length L */
            for (idx_t i = 0; i <= cur_deg; ++i) {
                seq_t val = coeff[i];
                if (val == 0) continue;
                for (idx_t j = 0; j < L; ++j) {
                    tmp[i + j] += val;
                }
            }

            /* copy back to coeff */
            for (idx_t i = 0; i <= new_deg; ++i) coeff[i] = tmp[i];
            cur_deg = new_deg;
        }
        free(tmp);
    }

    if (n > DEG) return (seq_t)0;
    return coeff[n];
}
