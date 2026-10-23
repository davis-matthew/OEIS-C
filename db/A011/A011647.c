/*
 * A011647  95th cyclotomic polynomial.
 * https://oeis.org/A011647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of the 95th cyclotomic polynomial Φ_95(x)
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The coefficient is 0 when n is outside 0..72. */

#include <stddef.h>
#include <stdbool.h>

/* Compute the coefficient a(n) of Φ_95(x).  The coefficients are stored
 * in a static array of size 73 (degree 72).  The computation is done
 * once on the first call.  Coefficients are integers -1, 0, or 1. */
seq_t a(idx_t n)
{
    /* Degree of Φ_95 is φ(95) = φ(5)·φ(19) = 4·18 = 72.  */
    static seq_t coeffs[73];
    static bool initialized = false;

    if (!initialized) {
        /* Denominator D(x) = (x-1)·Φ_5(x)·Φ_19(x)
         * Property: D(x) = -1 - x - x^2 - x^3 - x^4 + x^19 + x^20 + x^21 + x^22 + x^23
         * Degree 23, 24 coefficients. */
        static seq_t D[24] = {0};
        for (int i = 0; i < 5; ++i) D[i] = (seq_t)-1;
        for (int i = 19; i < 24; ++i) D[i] = (seq_t)1;
        /* Note: indices 5..18 are implicitly 0. */

        /* N(x) = x^95 - 1.  Length 96. */
        static seq_t N[96] = {0};
        N[0] = (seq_t)-1;
        N[95] = (seq_t)1;

        const int degN = 95;
        const int degD = 23;      /* degree of D(x) */
        const int degQ = degN - degD; /* 72 */

        static seq_t Q[73] = {0};

        /* Long division N / D.  N is modified in place. */
        for (int i = degN; i >= degD; --i) {
            seq_t coeff = N[i];
            Q[i - degD] = coeff;            /* coefficient of x^(i-degD) in Q */
            if (coeff != (seq_t)0) {
                for (int j = 0; j <= degD; ++j) {
                    N[i - degD + j] -= coeff * D[j];
                }
            }
        }

        /* Store the result in coeffs. */
        for (int i = 0; i <= degQ; ++i) {
            coeffs[i] = Q[i];
        }
        initialized = true;
    }

    return (n < 73) ? coeffs[(size_t)n] : (seq_t)0;
}
