/*
 * A203002  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A203001; by antidiagonals.
 * https://oeis.org/A203002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   T(r,k) = [x^k] det(M_r - x*I), k = 0..r, where M_r is the r-th
 *            principal submatrix of A203001, M[i][j] = Sum_{t=0}^{min(i,j)-1}
 *            F(i-t)^2*F(j-t)^2. Coefficients come from the Faddeev-LeVerrier
 *            recurrence: N_0 = I, c_r = 1; c_{r-s} = -tr(M*N_{s-1})/s, N_s =
 *            M*N_{s-1} + c_{r-s}*I; then T(r,k) = (-1)...
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A203002_mul_sq(seq_t *A, seq_t *B, idx_t r)
{
    seq_t *C = mem_new(seq_t, r * r);
    for (idx_t i = 0; i < r; i++)
        for (idx_t k = 0; k < r; k++) {
            seq_t x = A[i * r + k];
            for (idx_t j = 0; j < r; j++)
                C[i * r + j] += x * B[k * r + j];
        }
    return C;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* locate row r (length r+1) and position k (0-based, ascending powers) */
    idx_t r = 1, start = 1;
    while (start + r + 1 <= n) { start += r + 1; r++; }
    idx_t k = n - start;

    /* Fibonacci squares */
    seq_t *fs = mem_new(seq_t, r + 2);
    {
        seq_t f0 = 0, f1 = 1;
        for (idx_t i = 1; i <= r; i++) {
            fs[i] = f1 * f1;
            seq_t t = f0 + f1; f0 = f1; f1 = t;
        }
    }
    seq_t *A = mem_new(seq_t, r * r);
    for (idx_t i = 1; i <= r; i++)
        for (idx_t j = 1; j <= r; j++) {
            idx_t m = i < j ? i : j;
            seq_t s = 0;
            for (idx_t t = 0; t < m; t++) s += fs[i - t] * fs[j - t];
            A[(i - 1) * r + (j - 1)] = s;
        }

    /* Faddeev-LeVerrier: coefficient of x^k in det(xI - A) is reached at step r-k */
    idx_t steps = r - k;
    seq_t d = 1;
    if (steps > 0) {
        seq_t *N = mem_new(seq_t, r * r);
        for (idx_t i = 0; i < r; i++) N[i * r + i] = 1;
        for (idx_t s = 1; s <= steps; s++) {
            seq_t *P = A203002_mul_sq(A, N, r);
            seq_t tr = 0;
            for (idx_t i = 0; i < r; i++) tr += P[i * r + i];
            d = -tr / (seq_t)s;
            if (s < steps) {
                for (idx_t i = 0; i < r; i++) P[i * r + i] += d;
            }
            mem_free(N);
            N = P;
        }
        mem_free(N);
    }
    mem_free(A);
    mem_free(fs);
    /* det(A - xI) = (-1)^r det(xI - A) */
    if (r & 1) d = -d;
    return d;
}
