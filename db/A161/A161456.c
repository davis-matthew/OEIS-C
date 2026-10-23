/*
 * A161456  Number of reduced words of length n in the Weyl group A_8.
 * https://oeis.org/A161456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in Π_{k=1..8} (1 + x + … + x^k)
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* The generating function is the product of polynomials
       (1 + x + … + x^k) for k = 1 … 8.  The resulting polynomial
       has degree Σ_{k=1}^{8} k = 36.  The coefficients can be
       precomputed once and cached. */
    static seq_t coeff[37];         /* 0 … 36 */
    static bool built = false;
    const idx_t DEG = 36;

    if (!built) {
        for (idx_t i = 0; i <= DEG; ++i)
            coeff[i] = 0;
        coeff[0] = 1;               /* polynomial starts with 1 */

        idx_t cur = 0;  /* current maximum degree of the polynomial */
        for (idx_t k = 1; k <= 8; ++k) {
            /* Convolve the current polynomial with (1 + x + … + x^k) */
            for (idx_t t = cur; ; --t) {
                seq_t val = coeff[t];
                if (val) {
                    for (idx_t add = 1; add <= k; ++add) {
                        coeff[t + add] += val;
                    }
                }
                if (t == 0) break;   /* unsigned loop guard */
            }
            cur += k;
        }
        built = true;
    }

    return (n <= DEG) ? coeff[n] : 0;
}
