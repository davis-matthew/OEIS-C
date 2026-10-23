/*
 * A161571  Number of reduced words of length n in the Weyl group A_28.
 * https://oeis.org/A161571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 45, u128 n <= 406,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161571: Number of reduced words of length n in the Weyl group A_28 */
/* a(n) = coefficient of x^n in the polynomial product ∏_{k=1}^{28} (1 + x + … + x^k) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The maximal degree is 1+2+…+28 = 406.  All terms for n>406 vanish. */
    static int initialized = 0;
    static seq_t coeff[407];
    static seq_t tmp[407];

    if (!initialized) {
        size_t cur_max = 0;
        coeff[0] = (seq_t)1;
        initialized = 1;

        /* Multiply successively by (1 + x + … + x^k) for k = 1 … 28 */
        for (int k = 1; k <= 28; ++k) {
            size_t new_max = cur_max + k;

            /* zero temp array up to new_max */
            for (size_t i = 0; i <= new_max; ++i)
                tmp[i] = (seq_t)0;

            /* convolution */
            for (size_t i = 0; i <= cur_max; ++i) {
                seq_t v = coeff[i];
                for (int j = 0; j <= k; ++j)
                    tmp[i + j] += v;
            }

            /* copy back to coeff array */
            for (size_t i = 0; i <= new_max; ++i)
                coeff[i] = tmp[i];

            cur_max = new_max;
        }
    }

    if (n <= (idx_t)406)
        return coeff[(size_t)n];
    else
        return (seq_t)0;
}
