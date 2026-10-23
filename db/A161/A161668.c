/*
 * A161668  Number of reduced words of length n in the Weyl group A_43.
 * https://oeis.org/A161668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{43} (1-x^(k+1))/(1-x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Number of reduced words of length n in the Weyl group A_43.  */
seq_t a(idx_t n)
{
    /* The generating polynomial has degree 43*44/2 = 946. */
    enum { MAX_DEG = 43U * 44U / 2U };   /* 946 */
    static seq_t coeff[MAX_DEG + 1];
    static bool initialized = false;

    if (!initialized) {
        /* Zero‐initialize the coefficient array. */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            coeff[i] = 0;

        /* Start with the constant 1. */
        coeff[0] = 1;
        idx_t current_max = 0;

        /* Multiply successively by (1 + x + … + x^k) for k=1..43. */
        for (idx_t k = 1; k <= 43U; ++k) {
            /* Update from high power to low to avoid using freshly updated
               values in the same iteration. */
            for (idx_t i = current_max; ; --i) {
                seq_t val = coeff[i];
                if (val != 0) {
                    for (idx_t s = 1; s <= k; ++s)
                        coeff[i + s] += val;
                }
                if (i == 0) break;
            }
            current_max += k;
        }
        initialized = true;
    }

    /* Indices beyond the degree give zero. */
    if (n > MAX_DEG)
        return 0;

    return coeff[n];
}
