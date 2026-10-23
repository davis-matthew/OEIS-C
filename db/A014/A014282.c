/*
 * A014282  Inverse of 273rd cyclotomic polynomial.
 * https://oeis.org/A014282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of xⁿ in 1/Φ₍₂₇₃₎(x) * i.e. a(n) =
 *            coefficient of xⁿ in * ∏_{d|273} (1 - xᵈ)^(-μ(273/d)). * The
 *            coefficients are periodic with period 273
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[273];
    static int initialized = 0;

    if (!initialized)
    {
        seq_t cur[273];
        seq_t next[273];

        /* start with the constant 1 */
        for (size_t i = 0; i < 273; ++i) cur[i] = 0;
        cur[0] = 1;

        /* divisors of 273 and the corresponding exponents
         * e = -μ(273/d) :  +1 ↦ (1 - xᵈ),  -1 ↦ 1/(1 - xᵈ)  */
        const int divisors[7] = {1, 3, 7, 13, 21, 39, 91};
        const int exponents[7] = {+1, -1, -1, -1, +1, +1, +1};

        for (int step = 0; step < 7; ++step)
        {
            int d = divisors[step];
            int e = exponents[step];

            if (e > 0)               /* multiply by (1 - xᵈ) */
            {
                for (size_t i = 0; i < 273; ++i)
                {
                    seq_t val = cur[i];
                    if (i >= (size_t)d) val -= cur[i - d];
                    next[i] = val;
                }
            }
            else                     /* multiply by 1/(1 - xᵈ) */
            {
                for (size_t i = 0; i < 273; ++i)
                {
                    if (i < (size_t)d)
                        next[i] = cur[i];
                    else
                        next[i] = next[i - d] + cur[i];
                }
            }

            /* copy next back into cur for the next step */
            for (size_t i = 0; i < 273; ++i) cur[i] = next[i];
        }

        /* store the period cache */
        for (size_t i = 0; i < 273; ++i) period[i] = cur[i];
        initialized = 1;
    }

    return period[n % 273];
}
