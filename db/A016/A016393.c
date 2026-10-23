/*
 * A016393  371st cyclotomic polynomial.
 * https://oeis.org/A016393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 312, i16 n <= 312, i32 n <= 312, i64 n <= 312,
 *            i128 n <= 312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of Φ371(x) computed by polynomial long division of
   (x^371-1)(x-1) ÷ (x^7-1)(x^53-1). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t p   = 7;
    const idx_t q   = 53;
    const idx_t pq  = p * q;            /* 371 */
    const idx_t Ndeg = pq + 1;          /* 372 */
    const idx_t Ddeg = p + q;           /* 60 */
    const idx_t Qsize = Ndeg - Ddeg + 1;/* 313 */

    static bool initialized = false;
    static seq_t coeff[313];           /* Qsize is 313 */

    if (!initialized)
    {
        /* Numerator: (x^pq - 1)(x - 1) = 1 - x - x^pq + x^(pq+1) */
        seq_t N[373] = {0};
        N[0]      =  1;
        N[1]      = -1;
        N[pq]     = -1;
        N[pq + 1] =  1;

        /* Denominator: (x^p - 1)(x^q - 1) = 1 - x^p - x^q + x^(p+q) */
        seq_t D[61] = {0};
        D[0]      =  1;
        D[p]      = -1;
        D[q]      = -1;
        D[p + q]  =  1;

        /* Quotient array */
        seq_t Q[313] = {0};

        for (int i = (int)Ndeg; i >= (int)Ddeg; --i)
        {
            seq_t c = N[i];
            if (c != 0)
            {
                idx_t shift = i - Ddeg;       /* 0 ≤ shift ≤ 312 */
                Q[shift] = c;                 /* leading coefficient */

                for (size_t j = 0; j <= Ddeg; ++j)
                    N[shift + j] -= c * D[j];
            }
        }

        for (size_t i = 0; i < Qsize; ++i)
            coeff[i] = Q[i];

        initialized = true;
    }

    return (n < Qsize) ? coeff[n] : (seq_t)0;
}
