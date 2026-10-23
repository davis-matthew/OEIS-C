/*
 * A016404  413th cyclotomic polynomial.
 * https://oeis.org/A016404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the coefficient of x^n in the cyclotomic polynomial
 *            Φ_413(x) computed as (x-1)(x^413-1)/((x^7-1)(x^59-1))
 * Range:     i8 n <= 348, i16 n <= 348, i32 n <= 348, i64 n <= 348,
 *            i128 n <= 348, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define NUM_DEG   414   /* degree of numerator (x-1)(x^413-1) */
#define DEN_DEG   66    /* degree of denominator (x^7-1)(x^59-1) */
#define QUOT_DEG  348   /* degree of Φ_413(x) */

/* Return the n‑th coefficient of Φ_413(x).  The sequence is defined
   for n = 0 … QUOT_DEG; for larger n the value is zero. */
seq_t a(idx_t n)
{
    static seq_t coeff[QUOT_DEG + 1];
    static bool   initialized = false;

    if (!initialized) {
        /* Polynomials are stored with coefficient of x^i in index i. */
        static seq_t N[NUM_DEG + 1] = {0};   /* (x-1)(x^413-1) */
        static seq_t D[DEN_DEG + 1] = {0};   /* (x^7-1)(x^59-1) */

        /* Numerator:  x^414 - x^413 - x + 1 */
        N[NUM_DEG]     = 1;          /* x^414 */
        N[NUM_DEG - 1] = -1;         /* -x^413 */
        N[1]           = -1;         /* -x       */
        N[0]           = 1;          /* +1       */

        /* Denominator:  x^66 - x^59 - x^7 + 1 */
        D[DEN_DEG] = 1;          /* x^66 */
        D[59]     = -1;          /* -x^59 */
        D[7]      = -1;          /* -x^7  */
        D[0]      = 1;           /* +1    */

        /* Perform polynomial long division N / D. */
        static seq_t dividend[NUM_DEG + 1];
        for (idx_t i = 0; i <= NUM_DEG; ++i) dividend[i] = N[i];

        /* Quotient will be stored in coeff. */
        for (idx_t i = NUM_DEG; ; --i) {
            if (i < DEN_DEG) break;
            seq_t q = dividend[i];
            if (q != 0) {
                idx_t shift = i - DEN_DEG;
                coeff[shift] = q;   /* coefficient of x^shift in quotient */
                for (idx_t j = 0; j <= DEN_DEG; ++j) {
                    dividend[shift + j] -= q * D[j];
                }
            }
        }
        initialized = true;
    }

    if (n <= QUOT_DEG) return coeff[n];
    return (seq_t)0;
}
