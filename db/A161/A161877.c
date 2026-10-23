/*
 * A161877  Number of reduced words of length n in the Weyl group B_17.
 * https://oeis.org/A161877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..17} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 289, u128 n <= 289,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, not strictly required but harmless */

#define MAX_DEG   289   /* 17^2, maximum degree of the polynomial */

seq_t a(idx_t n)
{
    static seq_t coeff[MAX_DEG + 1];  /* coefficients of the product polynomial */
    static int   initialized = 0;    /* one‑time initialization flag */

    if (!initialized)
    {
        /* initialise all coefficients to 0 and start with 1 */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        idx_t cur_deg = 0;   /* current maximum degree of coeff */

        for (idx_t k = 1; k <= 17; ++k)
        {
            const idx_t step = 2 * k - 1;  /* degree of the factor (1 + … + x^step) */

            /* temporary storage for convolution results */
            static seq_t tmp[MAX_DEG + 1];
            /* zero out the relevant part of tmp */
            for (idx_t i = 0; i <= cur_deg + step; ++i)
                tmp[i] = 0;

            /* convolve coeff with the new factor */
            for (idx_t i = 0; i <= cur_deg; ++i)
            {
                const seq_t val = coeff[i];
                for (idx_t j = 0; j <= step; ++j)
                    tmp[i + j] = tmp[i + j] + val;
            }

            /* copy back the result into coeff */
            for (idx_t i = 0; i <= cur_deg + step; ++i)
                coeff[i] = tmp[i];

            cur_deg += step;
        }

        initialized = 1;
    }

    if (n > MAX_DEG)      /* coefficients beyond the polynomial’s degree are 0 */
        return 0;

    return coeff[n];
}
