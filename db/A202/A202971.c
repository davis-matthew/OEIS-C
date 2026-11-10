/*
 * A202971  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A202970; by antidiagonals.
 * https://oeis.org/A202971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   T(r,k) = coefficient of x^k in det(M_r - x*I), k = 0..r, where
 *            M_r is the r-th principal submatrix of A202970: M[i][j] =
 *            Sum_{t=1..min(i,j)} s(t)*s(|i-j|+t), s(t) = F(t+3) - 2. The
 *            characteristic polynomial comes from the Faddeev-LeVerrier
 *            method. The rows are read by antidiagonals
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A202971_charcoef(idx_t r, idx_t k)
{
    size_t R = (size_t)r;
    size_t nf = 2 * R + 5;
    seq_t *fib = mem_new(seq_t, nf);
    seq_t *A = mem_new(seq_t, R * R);
    seq_t *Mk = mem_new(seq_t, R * R);
    seq_t *AM = mem_new(seq_t, R * R);
    seq_t *c = mem_new(seq_t, R + 1);
    fib[0] = 0;
    fib[1] = 1;
    for (size_t i = 2; i < nf; i++) fib[i] = fib[i - 1] + fib[i - 2];

    for (size_t i = 1; i <= R; i++) {
        for (size_t j = 1; j <= R; j++) {
            size_t d = (i > j) ? (i - j) : (j - i);
            size_t m = (i < j) ? i : j;
            seq_t sum = 0;
            for (size_t t = 1; t <= m; t++) {
                seq_t s1 = fib[t + 3] - 2;
                seq_t s2 = fib[d + t + 3] - 2;
                sum += s1 * s2;
            }
            A[(i - 1) * R + (j - 1)] = sum;
        }
    }

    /* Faddeev-LeVerrier: c[] are coefficients of det(xI - A), c[R] = 1 */
    for (size_t i = 0; i < R; i++)
        for (size_t j = 0; j < R; j++)
            Mk[i * R + j] = (i == j) ? 1 : 0;
    c[R] = 1;
    for (size_t step = 1; step <= R; step++) {
        seq_t tr = 0;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < R; j++) {
                seq_t s = 0;
                for (size_t l = 0; l < R; l++)
                    s += A[i * R + l] * Mk[l * R + j];
                AM[i * R + j] = s;
            }
            tr += AM[i * R + i];
        }
        seq_t cc = -tr / (seq_t)step;
        c[R - step] = cc;
        for (size_t i = 0; i < R; i++)
            for (size_t j = 0; j < R; j++)
                Mk[i * R + j] = AM[i * R + j] + ((i == j) ? cc : 0);
    }
    /* det(A - xI) = (-1)^R det(xI - A); coefficient of x^k */
    seq_t res = c[(size_t)k];
    if (R & 1) res = -res;
    mem_free(fib);
    mem_free(A);
    mem_free(Mk);
    mem_free(AM);
    mem_free(c);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t pos = n - 1;
    idx_t r = 1;
    while (pos >= r + 1) {
        pos -= r + 1;
        r++;
    }
    return A202971_charcoef(r, pos);
}
