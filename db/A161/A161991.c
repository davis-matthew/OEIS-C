/*
 * A161991  Number of reduced words of length n in the Weyl group B_34.
 * https://oeis.org/A161991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k=1..34} (1 + x + … + x^(2k-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the coefficient of x^n in the generating function for Weyl group B_34 */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maximum degree of the polynomial is 34^2 = 1156 */
    static seq_t coeff[1157];
    static seq_t temp[1157];
    static bool init = false;

    if (!init) {
        coeff[0] = (seq_t)1;
        idx_t current = 0;

        for (idx_t k = 1; k <= (idx_t)34; ++k) {
            idx_t step   = 2 * k - 1;          /* length of the current factor minus one */
            idx_t new_max = current + step;    /* largest possible degree after this factor */

            /* zero temporary array up to new_max */
            for (idx_t i = 0; i <= new_max; ++i)
                temp[i] = (seq_t)0;

            /* convolution with an all‑ones kernel of length step+1 */
            for (idx_t i = 0; i <= current; ++i) {
                seq_t val = coeff[i];
                for (idx_t j = 0; j <= step; ++j)
                    temp[i + j] += val;
            }

            /* copy back to coeff array */
            for (idx_t i = 0; i <= new_max; ++i)
                coeff[i] = temp[i];

            current = new_max;
        }
        init = true;
    }

    return (n <= 1156) ? coeff[(size_t)n] : (seq_t)0;
}
