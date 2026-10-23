/*
 * A016344  170th cyclotomic polynomial.
 * https://oeis.org/A016344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 170th cyclotomic polynomial
 *            Φ_170(x)
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   mul_by_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The polynomial is defined by   Φ_170(x) = ∏_{d|170} (x^d – 1)^{μ(170/d)}   */
/* where μ is the Möbius function.  The coefficients are computed once and cached. */

#include <stddef.h>
#include "../utils/mul_by_factor.h"

/* Compute coefficient array of Φ_170(x) the first time this function is called.
   The coefficients are then returned for any subsequent call. */
seq_t a(idx_t n)
{
    /* 65 coefficients from degree 0 to 64 inclusive. */
    static seq_t coeff[65];
    static int initialized = 0;

    if (!initialized)
    {
        /* Divisors d of 170 where μ(170/d) = +1 (factor with exponent +1). */
        const int num_div[4] = { 2, 5, 17, 170 };
        /* Divisors d of 170 where μ(170/d) = –1 (factor with exponent –1). */
        const int den_div[4] = { 1, 10, 34, 85 };

        /* Temporary polynomials for numerator and denominator. */
        seq_t N[200] = {0};
        int degN = 0;
        N[0] = 1;                     /* start from polynomial 1 */

        seq_t D[200] = {0};
        int degD = 0;
        D[0] = 1;

        /* Build numerator: product of (x^d – 1) for d in num_div. */
        for (int i = 0; i < 4; ++i)
            mul_by_factor(N, &degN, num_div[i]);

        /* Build denominator: product of (x^d – 1) for d in den_div. */
        for (int i = 0; i < 4; ++i)
            mul_by_factor(D, &degD, den_div[i]);

        /* Exact division N / D → Q (the cyclotomic polynomial). */
        seq_t numer[200];
        for (int i = 0; i <= degN; ++i)
            numer[i] = N[i];

        int degQ = degN - degD;          /* should be 64 */
        seq_t Q[200] = {0};

        for (int i = degN; i >= degD; --i) {
            seq_t qcoeff = numer[i];
            Q[i - degD] = qcoeff;
            for (int j = 0; j <= degD; ++j) {
                numer[i - degD + j] -= qcoeff * D[j];
            }
        }

        /* Copy the resulting coefficients to the cache. */
        for (int i = 0; i <= degQ; ++i)
            coeff[i] = Q[i];

        initialized = 1;
    }

    /* Return coefficient if in range, otherwise 0. */
    if (n < 65)
        return coeff[(size_t)n];
    else
        return (seq_t)0;
}
