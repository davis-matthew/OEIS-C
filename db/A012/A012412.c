/*
 * A012412  cos(arctan(x)*exp(x))=1-1/2!*x^2-6/3!*x^3-15/4!*x^4+20/5!*x^5...
 * https://oeis.org/A012412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A012412: a(n) = n!* [x^n] cos(arctan(x)*exp(x)) */
/* Implements integer recurrence using the integer sequences
   D[n] = n!*g_n  (coefficients of G(x)=arctan(x)*exp(x)) and
   the generating function relation for sin and cos. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Allocate temporary arrays */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *D    = (seq_t*)malloc((n + 2) * sizeof(seq_t));   /* up to n+1 */
    seq_t *A    = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* a[0..n]   */
    seq_t *B    = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* sin coefficients*/

    /* factorials */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* Compute D[0..n] */
    D[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;   /* C(i,0) */
        for (idx_t j = 1; j <= i; ++j) {
            /* binom = C(i, j) */
            binom = (binom * (seq_t)(i - j + 1)) / (seq_t)j;
            if (j & 1) {               /* odd j */
                int sign = (((j-1) >> 1) & 1) ? -1 : 1;
                seq_t term = binom * fact[j-1];  /* binom * (j-1)! */
                if (sign > 0)
                    sum += term;
                else
                    sum -= term;
            }
        }
        D[i] = sum;
    }
    /* D[n+1] required for recurrence: it is zero by construction */
    D[n+1] = 0;

    /* Recurrence for a and sin coefficients */
    A[0] = 1;   /* cos(0)=1 */
    B[0] = 0;   /* sin(0)=0 */

    for (idx_t m = 0; m < n; ++m) {
        seq_t a_next = 0;
        seq_t b_next = 0;
        seq_t binom  = 1;   /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0) {
                binom = (binom * (seq_t)(m - k + 1)) / (seq_t)k;
            }

            /* term for a_{m+1} uses B[k] (sin coefficients) */
            seq_t termA = B[k] * D[m - k + 1];
            termA = termA * binom;
            a_next -= termA;

            /* term for b_{m+1} uses A[k] (cos coefficients) */
            seq_t termB = A[k] * D[m - k + 1];
            termB = termB * binom;
            b_next += termB;
        }
        A[m+1] = a_next;
        B[m+1] = b_next;
    }

    seq_t result = A[n];

    /* Free allocated memory */
    free(fact);
    free(D);
    free(A);
    free(B);

    return result;
}
