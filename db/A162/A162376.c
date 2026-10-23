/*
 * A162376  Number of reduced words of length n in the Weyl group D_29.
 * https://oeis.org/A162376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1+x+⋯+x^28) ·
 *            Π_{i=1}^{28}(1+x+⋯+x^{2i-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 42, u128 n <= 812,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define DEGREE 812

seq_t a(idx_t n)
{
    /* Compute the polynomial for the Weyl group D_29 once and cache its coefficients. */
    static seq_t coeff[DEGREE + 1];
    static int built = 0;

    if (!built)
    {
        /* Initialize coefficients to zero. */
        for (size_t i = 0; i <= DEGREE; ++i)
            coeff[i] = 0;
        coeff[0] = 1;  /* Start with the constant polynomial 1. */

        /* List of factor degrees: first f(29), then f(2), f(4), ..., f(56). */
        int mvals[29];
        mvals[0] = 29;
        for (int i = 1; i <= 28; ++i)
            mvals[i] = 2 * i;

        seq_t temp[DEGREE + 1];

        for (int idx = 0; idx < 29; ++idx)
        {
            int m = mvals[idx];            /* Degree of the current factor: f(m). */
            for (size_t i = 0; i <= DEGREE; ++i)  /* Clear temporary array. */
                temp[i] = 0;

            for (size_t i = 0; i <= DEGREE; ++i)
            {
                seq_t ci = coeff[i];
                if (ci == 0)
                    continue;
                for (int j = 0; j < m; ++j)
                {
                    size_t pos = i + (size_t)j;
                    if (pos > DEGREE)
                        break;
                    temp[pos] += ci;
                }
            }
            /* Copy result back into coeff. */
            for (size_t i = 0; i <= DEGREE; ++i)
                coeff[i] = temp[i];
        }
        built = 1;  /* Mark polynomial as built. */
    }

    if (n <= DEGREE)
        return coeff[(size_t)n];
    else
        return 0;  /* No reduced words longer than the maximum degree. */
}
