/*
 * A203954  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of A203953.
 * https://oeis.org/A203954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   row r (r >= 1), column j = 0..r: T(r,j) = coefficient of x^j in
 *            det(M_r - x*I), where M_r is the r-th principal submatrix of
 *            A203953, M(p,q) = Sum_{k=1..min(p,q)} s(p-k+1)*s(q-k+1), s(i) =
 *            2 - (i mod 2). The characteristic polynomial det(x*I - M_r) =
 *            Sum c_k x^k comes from the Faddeev-LeVerrier r...
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;
    unsigned long long r = 1, start = 1;
    while (N >= start + r + 1) { start += r + 1; r++; }
    unsigned long long j = N - start;   /* 0..r */
    int R = (int)r;

    seq_t *A  = mem_new(seq_t, R * R);
    seq_t *Mk = mem_new(seq_t, R * R);
    seq_t *P  = mem_new(seq_t, R * R);
    seq_t *c  = mem_new(seq_t, R + 1);

    for (int p = 1; p <= R; p++)
        for (int q = 1; q <= R; q++) {
            int m = p < q ? p : q;
            long long sum = 0;
            for (int k = 1; k <= m; k++) {
                int u = p - k + 1, v = q - k + 1;
                sum += (long long)(2 - (u % 2)) * (2 - (v % 2));
            }
            A[(p - 1) * R + (q - 1)] = (seq_t)sum;
        }

    /* Faddeev-LeVerrier: det(xI - A) = sum c[k] x^k, c[R] = 1 */
    c[R] = 1;
    for (int k = 1; k <= R; k++) {
        for (int x = 0; x < R; x++)
            for (int y = 0; y < R; y++) {
                seq_t s = 0;
                for (int z = 0; z < R; z++)
                    s += A[x * R + z] * Mk[z * R + y];
                if (x == y) s += c[R - k + 1];
                P[x * R + y] = s;
            }
        for (int i = 0; i < R * R; i++) Mk[i] = P[i];
        seq_t tr = 0;
        for (int x = 0; x < R; x++)
            for (int z = 0; z < R; z++)
                tr += A[x * R + z] * Mk[z * R + x];
        c[R - k] = -(tr / k);
    }

    /* det(A - xI) = (-1)^R det(xI - A); row lists ascending powers of x */
    seq_t res = c[(int)j];
    if (R & 1) res = -res;
    mem_free(A); mem_free(Mk); mem_free(P); mem_free(c);
    return res;
}
