/*
 * A204444  Permanent of the n-th principal submatrix of A204443.
 * https://oeis.org/A204444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * r_k * (n-k)!, where r_k is the k-th
 *            coefficient of the rook polynomial of the zero cells (i+j == 0
 *            mod 4) of the n X n matrix. That polynomial is the product of
 *            the rook polynomials of four complete blocks, with r_k(a x b
 *            block) = C(a,k)*C(b,k)*k!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A204444_mul_block(seq_t *P, size_t n, size_t a, size_t b,
                               seq_t *Cb, seq_t *fact)
{
    size_t m = (a < b ? a : b);
    size_t stride = n + 1;
    seq_t *B = mem_new(seq_t, m + 1);
    seq_t *Q = mem_new(seq_t, n + 1);
    size_t i, k;
    for (k = 0; k <= m; k++)
        B[k] = Cb[a * stride + k] * Cb[b * stride + k] * fact[k];
    for (i = 0; i <= n; i++) {
        if (P[i] == 0) continue;
        for (k = 0; k <= m && i + k <= n; k++)
            Q[i + k] += P[i] * B[k];
    }
    for (i = 0; i <= n; i++) P[i] = Q[i];
    mem_free(B);
    mem_free(Q);
    return 0;
}

seq_t a(idx_t n_)
{
    size_t n = (size_t)n_;
    size_t stride = n + 1;
    size_t i, j, k;
    size_t R[4] = {0, 0, 0, 0};
    for (i = 1; i <= n; i++) R[i % 4]++;

    seq_t *Cb = mem_new(seq_t, stride * stride);
    seq_t *fact = mem_new(seq_t, n + 1);
    seq_t *P = mem_new(seq_t, n + 1);

    for (i = 0; i <= n; i++) {
        Cb[i * stride] = 1;
        for (j = 1; j <= i; j++)
            Cb[i * stride + j] = Cb[(i - 1) * stride + j - 1]
                               + (j <= i - 1 ? Cb[(i - 1) * stride + j] : (seq_t)0);
    }
    fact[0] = 1;
    for (i = 1; i <= n; i++) fact[i] = fact[i - 1] * (seq_t)i;

    P[0] = 1;
    A204444_mul_block(P, n, R[1], R[3], Cb, fact);
    A204444_mul_block(P, n, R[3], R[1], Cb, fact);
    A204444_mul_block(P, n, R[2], R[2], Cb, fact);
    A204444_mul_block(P, n, R[0], R[0], Cb, fact);

    seq_t pos = 0, neg = 0;
    for (k = 0; k <= n; k++) {
        seq_t t = P[k] * fact[n - k];
        if (k % 2 == 0) pos += t; else neg += t;
    }
    seq_t res = pos - neg;

    mem_free(Cb);
    mem_free(fact);
    mem_free(P);
    return res;
}
