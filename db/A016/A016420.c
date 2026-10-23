/*
 * A016420  481st cyclotomic polynomial.
 * https://oeis.org/A016420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 432, i16 n <= 432, i32 n <= 432, i64 n <= 432,
 *            i128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in the 481st cyclotomic polynomial Φ_481(x) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t coeff[433];          /* coefficients for Φ_481, 0..432 */

    if (!initialized) {
        const idx_t N = 481;          /* n in Φ_n */
        const idx_t degD = 50;        /* degree of denominator (x^37-1)(x^13-1)  */
        const idx_t degN = 482;       /* degree of numerator (x^482 - ... )   */

        /* arrays on the stack; sizes are small (≤ 483) */
        seq_t D[51] = {0};            /* denominator polynomial */
        seq_t R[483] = {0};           /* dividend / intermediate */
        seq_t A[38] = {0};
        seq_t B[14] = {0};

        /* A(x) = x^37 - 1 */
        A[0] = (seq_t)-1;
        A[37] = (seq_t)1;

        /* B(x) = x^13 - 1 */
        B[0] = (seq_t)-1;
        B[13] = (seq_t)1;

        /* D(x) = A(x) * B(x) */
        for (idx_t i = 0; i <= 37; ++i) {
            for (idx_t j = 0; j <= 13; ++j) {
                D[i + j] += A[i] * B[j];
            }
        }

        /* N(x) = (x^481 - 1) * (x - 1) = x^482 - x^481 - x + 1 */
        R[0] = (seq_t)1;
        R[1] = (seq_t)-1;
        R[N] = (seq_t)-1;
        R[N + 1] = (seq_t)1;          /* term x^482 */

        /* Long division: N(x) / D(x)  →  Φ_481(x)  (degree 432) */
        /* i runs from 482 down to 50 */
        for (idx_t i = degN; ; --i) {
            seq_t c = R[i];
            coeff[i - degD] = c;              /* quotient coefficient */

            if (c != (seq_t)0) {
                for (idx_t j = 0; j <= degD; ++j) {
                    R[i - degD + j] -= c * D[j];
                }
            }

            if (i == (idx_t)degD) break;     /* finished all terms */
        }

        initialized = true;
    }

    /* return requested coefficient */
    if (n <= 432)  return coeff[(idx_t)n];
    else           return (seq_t)0;
}
