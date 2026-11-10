/*
 * A202678  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A185957; by antidiagonals.
 * https://oeis.org/A202678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   row r (r >= 1) lists the coefficients of p(r,x) = det(A_r - x*I)
 *            in ascending powers of x, for x^0..x^r. Here A_r is the r-th
 *            principal submatrix of A185957: A(i,j) = Sum_{k=1..min(i,j)}
 *            T(i-k+1)*T(j-k+1), with T(m) = m*(m+1)/2. The coefficients come
 *            from the Faddeev-LeVerrier recurrence: M_1 = I...
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A202678_tri(idx_t m)
{
    seq_t x = (seq_t)m;
    return x * (x + 1) / 2;
}

static seq_t A202678_entry(idx_t i, idx_t j)
{
    idx_t mn = (i < j) ? i : j;
    seq_t sum = 0;
    for (idx_t k = 1; k <= mn; ++k)
        sum += A202678_tri(i - k + 1) * A202678_tri(j - k + 1);
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t r = 1;
    while ((r * (r + 3)) / 2 < n)
        ++r;
    idx_t pos = n - ((r - 1) * (r + 2)) / 2 - 1; /* 0..r, power of x */
    idx_t target = r - pos;                       /* index into c_k */
    seq_t sign = (r % 2 == 0) ? (seq_t)1 : (seq_t)-1;
    if (target == 0)
        return sign;

    size_t R = (size_t)r;
    seq_t *A = mem_new(seq_t, R * R);
    seq_t *M = mem_new(seq_t, R * R);
    seq_t *N = mem_new(seq_t, R * R);
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < R; ++j)
            A[i * R + j] = A202678_entry((idx_t)(i + 1), (idx_t)(j + 1));

    seq_t c = 1;
    for (idx_t k = 1; k <= target; ++k) {
        /* N = A*M + c*I  (M_1 = I when k == 1) */
        for (size_t i = 0; i < R; ++i) {
            for (size_t j = 0; j < R; ++j) {
                seq_t s = 0;
                if (k > 1) {
                    for (size_t t = 0; t < R; ++t)
                        s += A[i * R + t] * M[t * R + j];
                }
                if (i == j)
                    s += c;
                N[i * R + j] = s;
            }
        }
        seq_t tr = 0;
        for (size_t i = 0; i < R; ++i)
            for (size_t j = 0; j < R; ++j)
                tr += A[i * R + j] * N[j * R + i];
        c = -tr / (seq_t)k;
        seq_t *tmp = M; M = N; N = tmp;
    }
    mem_free(A);
    mem_free(M);
    mem_free(N);
    return sign * c;
}
