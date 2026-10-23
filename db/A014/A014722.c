/*
 * A014722  Inverse of 713th cyclotomic polynomial.
 * https://oeis.org/A014722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) gives the coefficient of x^n in the power series 1/Φ₇₁₃(x).
 *            The coefficients are periodic with period 713
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static workspace: compute only once */
    static seq_t phi[661];           /* Φ₇₁₃(x) = Σ_{i=0}^{660} φ[i] x^i   (degree 660) */
    static seq_t a_seq[713];         /* first 713 terms of the sequence */
    static int initialized = 0;

    if (!initialized)
    {
        /* 0.  Build the divisor polynomial D(x) = (x-1)(x^22+...+1)(x^30+...+1) */
        /* sizes: (x-1): 2, Φ_23: 23, Φ_31: 31. Final degree 53. */
        seq_t d1[2] = {-1, 1};           /*  x - 1 */
        seq_t ph23[23];
        seq_t ph31[31];
        seq_t temp1[24]; /* (x-1) * Φ_23  -> degree 23 */
        seq_t D[54];     /* final divisor, degree 53 */

        /* fill Φ_23 and Φ_31 */
        for (int i = 0; i < 23; ++i) ph23[i] = 1;
        for (int i = 0; i < 31; ++i) ph31[i] = 1;

        /* temp1 = d1 * ph23 */
        for (int i = 0; i <= 23; ++i) temp1[i] = 0;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 23; ++j)
                temp1[i + j] += d1[i] * ph23[j];

        /* D = temp1 * ph31 */
        for (int i = 0; i <= 53; ++i) D[i] = 0;
        for (int i = 0; i <= 23; ++i)
            for (int j = 0; j < 31; ++j)
                D[i + j] += temp1[i] * ph31[j];

        /* 1.  Numerator: N(x) = x^713 - 1  */
        const int DEG_NUM = 713;
        seq_t num[714] = {0};
        num[DEG_NUM] = 1;
        num[0] = -1;

        /* 2.  Division: Φ₇₁₃(x) = N(x) / D(x)   (D is monic)
           Quotient has degree 660 (coefficients phi[0..660]) */
        for (int i = DEG_NUM; i >= 53; --i)          /* iterate from high to low */
        {
            seq_t coef = num[i];                    /* coefficient for current position */
            if (coef == 0) continue;
            int exp = i - 53;                       /* exponent of the quotient term */
            phi[exp] = coef;

            /* subtract coef * D(x) shifted by exp from num[] */
            for (int j = 0; j <= 53; ++j)
                num[exp + j] -= coef * D[j];
        }
        /* At this point num[0..52] should be zero (remainder). */

        /* 3.  Compute the first 713 terms of the inverse series using the recurrence
               Φ₇₁₃(0) * a₀ = 1  =>  a₀ = 1
               For k >= 1:
                   Σ_{j=0}^{min(660,k)} φ_j * a_{k-j} = 0
               =>  a_k = - Σ_{j=1}^{min(660,k)} φ_j * a_{k-j}  */
        a_seq[0] = 1;
        for (int k = 1; k < 713; ++k)
        {
            seq_t sum = 0;
            int maxj = k < 660 ? k : 660;
            for (int j = 1; j <= maxj; ++j)
                sum += phi[j] * a_seq[k - j];
            a_seq[k] = -sum;
        }

        initialized = 1;
    }

    /* Return the value for the requested index (periodic with length 713). */
    return a_seq[(size_t)n % 713];
}
