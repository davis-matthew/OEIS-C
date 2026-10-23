/*
 * A014446  Inverse of 437th cyclotomic polynomial.
 * https://oeis.org/A014446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series of 1 / Φ₄₃₇(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Φ₄₃₇ is the 437th cyclotomic polynomial; a(n) is periodic with period 437) */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[437];

    if (!initialized) {
        /* degrees */
        const size_t N       = 437;
        const size_t degP    = 438;  /* degree of numerator (x-1)(x^437-1) */
        const size_t degD    = 42;   /* degree of denominator (x^19-1)(x^23-1) */
        const size_t degPhi  = 396;  /* φ(437) = (19-1)(23-1) */

        /* ---- compute Φ₄₃₇(x) coefficients q[0..396] ---- */
        seq_t num[439] = {0};   /* numerator coefficients of size 439 */
        seq_t den[43]  = {0};   /* denominator coefficients of size 43 */
        seq_t rem[439] = {0};   /* working remainder during division */
        seq_t phi[397] = {0};   /* result coefficients (size 397) */

        /* numerator = (x-1)(x^437-1) = x^438 - x^437 - x + 1 */
        num[438] =  1;
        num[437] = -1;
        num[1]   = -1;
        num[0]   =  1;

        /* denominator = (x^19-1)(x^23-1) = x^42 - x^23 - x^19 + 1 */
        den[42] =  1;
        den[23] = -1;
        den[19] = -1;
        den[0]  =  1;

        /* copy numerator into remainder */
        for (size_t i = 0; i < sizeof(rem)/sizeof(*rem); ++i) rem[i] = num[i];

        /* polynomial long division: rem / den yields φ(x) coefficients */
        for (int i = (int)degP; i >= (int)degD; --i) {
            seq_t coeff = rem[i];
            if (coeff != 0) {
                size_t idx = (size_t)(i - degD);
                phi[idx] = coeff;
                for (size_t j = 0; j <= degD; ++j) {
                    rem[idx + j] -= coeff * den[j];
                }
            }
        }

        /* ---- compute the period of a(n) ---- */
        period[0] = 1;   /* a(0) = 1 */
        for (size_t k = 1; k < 437; ++k) {
            seq_t sum = 0;
            size_t max = (k < degPhi) ? k : degPhi;
            for (size_t j = 1; j <= max; ++j) {
                sum += phi[j] * period[k - j];
            }
            period[k] = -sum;
        }

        initialized = true;
    }

    /* return the value using periodicity */
    size_t idx = (size_t)(n % 437ULL);
    return period[idx];
}
