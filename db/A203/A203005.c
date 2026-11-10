/*
 * A203005  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of A115255 (in square
 *          format); by antidiagonals.
 * https://oeis.org/A203005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   row N (N >= 1) lists the coefficients of det(M_N - x*I) in
 *            ascending powers of x, where M_N(i,j) = Sum_{k=0..min(i,j)}
 *            C(2(i-k),i-k)*C(2(j-k),j-k), 0 <= i,j < N (A115255 square). a(n)
 *            = coefficient of x^k in row N, where n = N(N+1)/2 + k, 0 <= k <=
 *            N. Computed with the Faddeev-LeVerrier recurrence
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-07)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t A203005_T(idx_t i, idx_t j)
{
    idx_t max_t = (j <= (i - j)) ? j : (i - j);
    seq_t sum = 0;
    for (idx_t t = 0; t <= max_t; ++t) {
        seq_t b1 = binomial(2 * j - 2 * t, j - t);
        seq_t b2 = binomial(2 * i - 2 * j - 2 * t, i - j - t);
        sum += b1 * b2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t r = 1;
    while (m >= r + 1) {
        m -= r + 1;
        r++;
    }
    size_t N = (size_t)r;
    size_t k0 = (size_t)m;

    seq_t *A  = mem_new(seq_t, N * N);
    seq_t *M  = mem_new(seq_t, N * N);
    seq_t *P  = mem_new(seq_t, N * N);
    seq_t *c  = mem_new(seq_t, N + 1);

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            A[i * N + j] = A203005_T((idx_t)(i + j), (idx_t)j);

    /* c[k] = coefficient of x^k in det(x*I - A) */
    c[N] = 1;
    for (size_t k = 1; k <= N; k++) {
        if (k == 1) {
            for (size_t i = 0; i < N * N; i++) P[i] = 0;
        } else {
            for (size_t i = 0; i < N; i++)
                for (size_t j = 0; j < N; j++) {
                    seq_t s = 0;
                    for (size_t l = 0; l < N; l++)
                        s += A[i * N + l] * M[l * N + j];
                    P[i * N + j] = s;
                }
        }
        for (size_t i = 0; i < N; i++)
            P[i * N + i] += c[N - k + 1];
        for (size_t i = 0; i < N * N; i++) M[i] = P[i];

        seq_t tr = 0;
        for (size_t i = 0; i < N; i++)
            for (size_t l = 0; l < N; l++)
                tr += A[i * N + l] * M[l * N + i];
        c[N - k] = -tr / (seq_t)k;
    }

    /* det(A - x*I) = (-1)^N det(x*I - A) */
    seq_t res = c[k0];
    if (N & 1) res = -res;

    mem_free(A);
    mem_free(M);
    mem_free(P);
    mem_free(c);
    return res;
}
