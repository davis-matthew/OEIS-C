/*
 * A016335  143rd cyclotomic polynomial.
 * https://oeis.org/A016335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of Φ_143(x) at degree n, where Φ_143(x) =
 *            (x^144 - x^143 - x + 1) / ((x^11 - 1)(x^13 - 1))
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Lazy static initialization of the coefficient array. */
    static int initialized = 0;
    static seq_t coeff[121];          /* φ(143)+1 = 121 coefficients          */

    if (!initialized) {
        /* Constants: degrees of the polynomials involved. */
        #define NUM_DEG   144      /* degree of (x^144 - x^143 - x + 1) */
        #define DIV1_DEG  11       /* degree of (x^11 - 1)               */
        #define DIV2_DEG  13       /* degree of (x^13 - 1)               */
        #define Q1_DEG   (NUM_DEG - DIV1_DEG)
        #define Q2_DEG   (Q1_DEG - DIV2_DEG)

        /* Temporary coefficient arrays.  All are statically sized. */
        seq_t num[NUM_DEG + 1] = {0};
        seq_t div1[DIV1_DEG + 1] = {0};
        seq_t div2[DIV2_DEG + 1] = {0};
        seq_t q1[Q1_DEG + 1] = {0};
        seq_t q2[Q2_DEG + 1] = {0};
        seq_t tmp[NUM_DEG + 1] = {0};
        seq_t tmp2[Q1_DEG + 1] = {0};

        /* Num: (x^144 - x^143 - x + 1) */
        num[0]  =  1;
        num[1]  = -1;
        num[143] = -1;
        num[144] =  1;

        /* Div1: (x^11 - 1) */
        div1[0] = -1;
        div1[DIV1_DEG] = 1;

        /* Div2: (x^13 - 1) */
        div2[0] = -1;
        div2[DIV2_DEG] = 1;

        /* First division: tmp / div1 -> q1 */
        for (int i = 0; i <= NUM_DEG; ++i) tmp[i] = num[i];
        for (int j = Q1_DEG; j >= 0; --j) {
            seq_t coeff_q = tmp[j + DIV1_DEG];
            q1[j] = coeff_q;
            if (coeff_q != 0) {
                for (int k = 0; k <= DIV1_DEG; ++k) {
                    if (div1[k] != 0)
                        tmp[k + j] -= coeff_q * div1[k];
                }
            }
        }

        /* Second division: how divide q1 by div2 -> q2 */
        for (int i = 0; i <= Q1_DEG; ++i) tmp2[i] = q1[i];
        for (int j = Q2_DEG; j >= 0; --j) {
            seq_t coeff_q = tmp2[j + DIV2_DEG];
            q2[j] = coeff_q;
            if (coeff_q != 0) {
                for (int k = 0; k <= DIV2_DEG; ++k) {
                    if (div2[k] != 0)
                        tmp2[k + j] -= coeff_q * div2[k];
                }
            }
        }

        /* Store the final coefficients (x^0 … x^120). */
        for (int i = 0; i <= Q2_DEG; ++i) coeff[i] = q2[i];

        /* Flag that initialization is finished. */
        initialized = 1;
    }

    /* Return the requested coefficient or 0 when n is out of range. */
    if (n < 121u) {
        return coeff[(size_t)n];
    }
    return (seq_t)0;
}
