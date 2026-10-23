/*
 * A016349  185th cyclotomic polynomial.
 * https://oeis.org/A016349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 185th cyclotomic polynomial
 *            Φ_185(x)
 * Range:     i8 n <= 144, i16 n <= 144, i32 n <= 144, i64 n <= 144,
 *            i128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#define DEGREE 144   /* φ(185) = 144 */

seq_t a(idx_t n)
{
    static seq_t coeff[DEGREE + 1];
    static int initialized = 0;

    if (!initialized) {
        /* Work with larger intermediate type to avoid overflow for small seq_t. */
        static long long N[187];   /* numerator (degree 186) */
        static long long D[43];    /* denominator (degree 42) */
        static long long Q[145];   /* quotient (degree 144) */
        const idx_t degN = 186;
        const idx_t degD = 42;

        /* --- Build numerator N(x) = (x^185 - 1)(x - 1) = x^186 - x^185 - x + 1 --- */
        for (idx_t i = 0; i <= degN; ++i) N[i] = 0;
        N[186] = 1;
        N[185] = -1;
        N[1]  = -1;
        N[0]  = 1;

        /* --- Build denominator D(x) = (x^37 - 1)(x^5 - 1) = x^42 - x^37 - x^5 + 1 --- */
        for (idx_t i = 0; i <= degD; ++i) D[i] = 0;
        D[42] = 1;
        D[37] = -1;
        D[5]  = -1;
        D[0]  = 1;

        /* --- Long division N/D --- */
        for (idx_t i = 0; i <= degN - degD; ++i) Q[i] = 0;
        for (idx_t i = degN; i >= degD; --i) {
            long long coeff_q = N[i];          /* N[i] is the current leading term */
            idx_t pos = i - degD;              /* position of the quotient coefficient */
            Q[pos] = coeff_q;

            /* Subtract coeff_q * x^pos * D from N */
            if (coeff_q != 0) {
                for (idx_t j = 0; j <= degD; ++j) {
                    N[pos + j] -= coeff_q * D[j];
                }
            }
        }

        /* --- Copy result into static coeff array of type seq_t --- */
        for (idx_t i = 0; i <= DEGREE; ++i) coeff[i] = (seq_t)Q[i];
        initialized = 1;
    }

    return (n <= DEGREE) ? coeff[(size_t)n] : (seq_t)0;
}
