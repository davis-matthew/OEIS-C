/*
 * A015758  Inverse of 1749th cyclotomic polynomial.
 * https://oeis.org/A015758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n) is the coefficient sequence of the formal power
 *  series 1/Φ_1749(x) where Φ_1749(x) is the 1749‑th cyclotomic
 *  polynomial.  The sequence is periodic with period 1749 and can
 *  be computed by the recurrence
 *
 *      a(0) = 1
 *      a(n) = –∑_{k=1}^{φ(1749)} c(k)·a(n−k)          for n > 0
 *
 *  where c(k) are the coefficients of Φ_1749(x) and φ(1749)=1040.
 *  This implementation constructs Φ_1749(x) by dividing
 *  (x^d − 1) for each divisor d of 1749 and then computes the
 *  first 1749 terms of the inverse series.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum { PERIOD = 1749, MAX_D = 1750, NUM_DIV = 8, COEF_SIZE = 1041 };

/*  a(idx_t n):  return the nth term of the sequence  */
seq_t a(idx_t n)
{
    /* period values  */
    static seq_t period[PERIOD] = {0};

    /* coefficients of Φ_1749(x)  */
    static seq_t coeff[COEF_SIZE] = {0};

    static bool initialized = false;

    if (!initialized)
    {
        /* divisors of 1749 */
        const uint64_t divs[NUM_DIV] = {1,3,11,33,53,159,583,1749};
        int deg[NUM_DIV] = {0};

        /* polynomials for each divisor: use 64‑bit intermediates  */
        static int64_t poly[NUM_DIV][MAX_D] = {0};

        /* build Φ_d for each divisor d */
        for (int i = 0; i < NUM_DIV; ++i)
        {
            uint64_t d = divs[i];
            deg[i] = (int)d;

            /* initialise (x^d – 1) */
            poly[i][0]        = -1;          /* constant term   */
            poly[i][(int)d]   = 1;           /* x^d term        */

            /* divide by earlier Φ_e that divide d */
            for (int j = 0; j < i; ++j)
            {
                if (d % divs[j] != 0)   /* not a divisor, skip  */
                    continue;

                int dd = deg[j];         /* degree of Φ_e      */
                int dm = deg[i];         /* current degree     */
                int newd = dm - dd;      /* degree after division */

                /* locate quotient Q: Q = poly[i] / poly[j]  (monic) */
                static int64_t q[MAX_D];
                for (int k = 0; k <= newd; ++k) q[k] = 0;

                for (int ii = dm; ii >= dd; --ii)
                {
                    int64_t lead = poly[i][ii];
                    if (lead == 0)
                        continue;
                    int shift = ii - dd;
                    q[shift] += lead;            /* quotient term  */

                    /* poly[i] -= lead * (x^shift * poly[j])  */
                    for (int jj = 0; jj <= dd; ++jj)
                        poly[i][shift + jj] -= lead * poly[j][jj];
                }

                /* copy quotient back into poly[i] and truncate rest */
                for (int k = 0; k <= newd; ++k) poly[i][k] = q[k];
                for (int k = newd + 1; k <= dm; ++k) poly[i][k] = 0;
                deg[i] = newd;
            }
        }

        /* the last divisor 1749 gives Φ_1749(x)  */
        int phi_deg = deg[NUM_DIV - 1];
        for (int k = 0; k <= phi_deg; ++k)
            coeff[k] = (seq_t)poly[NUM_DIV - 1][k];

        /* compute the period of the inverse series 1/Φ_1749(x)  */
        period[0] = 1;
        for (int m = 1; m < PERIOD; ++m)
        {
            int64_t sum = 0;
            int up = (m < phi_deg) ? m : phi_deg;
            for (int k = 1; k <= up; ++k)
                sum += (int64_t)coeff[k] * (int64_t)period[m - k];
            period[m] = (seq_t)(-sum);
        }

        initialized = true;
    }

    /* return the nth term (periodic of length PERIOD)  */
    return period[(size_t)(n % PERIOD)];
}
