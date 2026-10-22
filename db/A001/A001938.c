/*
 * A001938  Expansion of k/(4*q^(1/2)) in powers of q, where k defined by
 *          sqrt(k) = theta_2(0, q)/theta_3(0, q).
 * https://oeis.org/A001938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. A(x) = psi(x^2)^2 / phi(x)^2, where psi(x^2) = Sum_{k>=0}
 *            x^(k*(k+1)) and phi(x) = 1 + 2*Sum_{k>=1} x^(k^2); computed by
 *            forming psi(x^2)^2 and dividing twice by the sparse series
 *            phi(x)
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 38, i64 n <= 131, i128 n <= 469,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *B = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *C = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *A = (seq_t *)calloc(len, sizeof(seq_t));
    idx_t i, j, k;
    seq_t result;

    for (i = 0; i < len; i++) {
        B[i] = (seq_t)0;
        C[i] = (seq_t)0;
        A[i] = (seq_t)0;
    }

    /* B = psi(x^2)^2 */
    for (j = 0; j * (j + 1) <= n; j++) {
        idx_t pj = j * (j + 1);
        for (k = 0; pj + k * (k + 1) <= n; k++) {
            B[pj + k * (k + 1)] += 1;
        }
    }

    /* C = B / phi(x) */
    for (i = 0; i < len; i++) {
        seq_t s = B[i];
        for (k = 1; k * k <= i; k++) {
            s -= (seq_t)2 * C[i - k * k];
        }
        C[i] = s;
    }

    /* A = C / phi(x) */
    for (i = 0; i < len; i++) {
        seq_t s = C[i];
        for (k = 1; k * k <= i; k++) {
            s -= (seq_t)2 * A[i - k * k];
        }
        A[i] = s;
    }

    result = A[n];
    free(B);
    free(C);
    free(A);
    return result;
}
