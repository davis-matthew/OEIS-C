/*
 * A202870  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A202869; by antidiagonals.
 * https://oeis.org/A202870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   row r of the table holds the coefficients of x^0..x^r of p(r,x)
 *            = det(A_r - x*I), where A_r is the r-th principal submatrix of
 *            A202869, A_r(i,j) = Sum_{t=1..min(i,j)} s(i-t+1)*s(j-t+1) and s
 *            = A000201 (lower Wythoff). Row r starts at n = (r-1)*(r+2)/2 +
 *            1. The coefficients come from the Faddeev-L...
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static unsigned long long A202870_isqrt(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1ULL << 32;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    /* row r covers n in ((r-1)(r+2)/2, r(r+3)/2] */
    idx_t r = 1;
    while (r * (r + 3) / 2 < n) r++;
    idx_t k = n - 1 - (r - 1) * (r + 2) / 2;   /* power of x, 0..r */
    size_t N = (size_t)r;

    seq_t *s = mem_new(seq_t, N + 2);
    for (size_t m = 1; m <= N; m++) {
        unsigned long long v = A202870_isqrt(5ULL * (unsigned long long)m * (unsigned long long)m);
        s[m] = (seq_t)((unsigned long long)((m + v) / 2));
    }
    seq_t *A = mem_new(seq_t, N * N);
    seq_t *M = mem_new(seq_t, N * N);
    seq_t *AM = mem_new(seq_t, N * N);
    for (size_t i = 1; i <= N; i++)
        for (size_t j = 1; j <= N; j++) {
            size_t mn = i < j ? i : j;
            seq_t sum = 0;
            for (size_t t = 1; t <= mn; t++)
                sum += s[i - t + 1] * s[j - t + 1];
            A[(i - 1) * N + (j - 1)] = sum;
        }
    for (size_t i = 0; i < N; i++) M[i * N + i] = 1;

    seq_t result = 0;
    if (k == (idx_t)N) result = 1;
    for (size_t step = 1; step <= N; step++) {
        for (size_t i = 0; i < N; i++)
            for (size_t j = 0; j < N; j++) {
                seq_t sum = 0;
                for (size_t t = 0; t < N; t++)
                    sum += A[i * N + t] * M[t * N + j];
                AM[i * N + j] = sum;
            }
        seq_t tr = 0;
        for (size_t i = 0; i < N; i++) tr += AM[i * N + i];
        seq_t c = -tr / (seq_t)step;           /* coefficient of x^(N-step) */
        if ((idx_t)(N - step) == k) result = c;
        if (step < N)
            for (size_t i = 0; i < N; i++)
                for (size_t j = 0; j < N; j++)
                    M[i * N + j] = AM[i * N + j] + (i == j ? c : (seq_t)0);
    }
    mem_free(s);
    mem_free(A);
    mem_free(M);
    mem_free(AM);
    if (N % 2 == 1) result = -result;          /* det(A - xI) = (-1)^N det(xI - A) */
    return result;
}
