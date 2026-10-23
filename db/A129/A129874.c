/*
 * A129874  Sequence h_n arising in enumeration of arrays of directed blocks
 *          (see Quaintance reference for precise definition).
 * https://oeis.org/A129874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (8*x^2-1)A^3 + (12*x^2-2*x-2)A^2 + (10*x^2-1)A +
 *            (8*x^3+9*x^2+2*x)=0 Recurrence for the coefficients a(n)
 *            (a(1)=2, a(2)=1, ...) derived from the cubic equation: a_m =
 *            8*a_{m-2}^* + 12*sum_{i+j=m-2} a_i*a_j + 10*a_{m-2} + const_m -
 *            (a_m^* + 2*sum_{i+j=m} a_i*a_j + 2*sum_{i+j=m-1} a_i*a_j) where
 *            a_...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* sequence function */
seq_t a(idx_t n)
{
    /* static cache for dynamic programming */
    static seq_t *A   = NULL;   /* a values        */
    static seq_t *C1  = NULL;   /* triple sums    */
    static seq_t *C2  = NULL;   /* pair sums      */
    static idx_t   maxN = 0;    /* largest n computed */

    if (n == 0) return 0;       /* out of range */

    if (n > maxN) {
        idx_t newN = n;

        /* grow the arrays */
        A   = (seq_t *)realloc(A,  (newN + 1) * sizeof(seq_t));
        C1  = (seq_t *)realloc(C1, (newN + 1) * sizeof(seq_t));
        C2  = (seq_t *)realloc(C2, (newN + 1) * sizeof(seq_t));

        /* initialise new entries */
        for (idx_t i = maxN + 1; i <= newN; ++i) {
            A[i]  = 0;
            C1[i] = 0;
            C2[i] = 0;
        }

        /* compute terms from maxN+1 up to newN */
        for (idx_t m = maxN + 1; m <= newN; ++m) {
            /* --- compute pair sum C2[m] --- */
            seq_t sumC2 = 0;
            for (idx_t i = 1; i < m; ++i) {
                sumC2 += A[i] * A[m - i];
            }
            C2[m] = sumC2;

            /* --- compute triple sum C1[m] --- */
            seq_t sumC1 = 0;
            if (m >= 3) {
                for (idx_t k = 1; k <= m - 2; ++k) {
                    sumC1 += A[k] * C2[m - k];
                }
            }
            C1[m] = sumC1;

            /* --- constant term from 2x + 9x^2 + 8x^3 --- */
            seq_t const_term = 0;
            if (m == 1) const_term = 2;
            else if (m == 2) const_term = 9;
            else if (m == 3) const_term = 8;

            /* --- positive part of the recurrence --- */
            seq_t pos = const_term;

            /* 8 * C1[m-2] */
            pos += C1[(m >= 3) ? (m - 2) : 0] * (seq_t)8;

            /* 12 * C2[m-2] */
            pos += C2[(m >= 3) ? (m - 2) : 0] * (seq_t)12;

            /* 10 * A[m-2] */
            pos += A[(m >= 3) ? (m - 2) : 0] * (seq_t)10;

            /* --- negative part of the recurrence --- */
            seq_t neg = C1[m];
            /* 2 * C2[m] */
            neg += C2[m] * (seq_t)2;
            /* 2 * C2[m-1] */
            neg += C2[(m >= 2) ? (m - 1) : 0] * (seq_t)2;

            /* result a[m] = pos - neg (guaranteed non‑negative) */
            A[m] = pos - neg;
        }
        maxN = newN;
    }
    return A[n];
}
