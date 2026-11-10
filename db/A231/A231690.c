/*
 * A231690  Cardinalities of the sub-operad FF_6 of the operad MFF.
 * https://oeis.org/A231690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = serreverse(A213667(-x))(-x), i.e. A satisfies x =
 *            Sum_{k>=1} (-1)^(k+1) * b(k) * A(x)^k with b = A213667 (b(1)=1,
 *            b(2)=6, b(3)=16, b(k) = 2*b(k-1) + b(k-2) + 2). So a(1) = 1 and
 *            a(m) = Sum_{k=2..m} (-1)^k * b(k) * [x^m] A(x)^k. The code
 *            builds the power table column by column and never div...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 278 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A231690_cache = NULL;
static size_t A231690_cnt = 0;   /* a(1..cnt) valid */

static void A231690_build(size_t N)
{
    seq_t *b = mem_new(seq_t, N + 2);
    seq_t *av = mem_new(seq_t, N + 1);
    seq_t *P = mem_new(seq_t, (N + 1) * (N + 1));   /* P[k*(N+1)+j] = [x^j] A^k */
    size_t W = N + 1;

    b[1] = (seq_t)1;
    if (N >= 2) b[2] = (seq_t)6;
    if (N >= 3) b[3] = (seq_t)16;
    for (size_t i = 4; i <= N; ++i)
        b[i] = (seq_t)2 * b[i - 1] + b[i - 2] + (seq_t)2;

    av[1] = (seq_t)1;
    P[1 * W + 1] = (seq_t)1;
    for (size_t m = 2; m <= N; ++m) {
        /* [x^m] A^k for k = 2..m, using a(1..m-1) */
        for (size_t k = 2; k <= m; ++k) {
            seq_t s = (seq_t)0;
            for (size_t i = 1; i + k <= m + 1; ++i)
                s += av[i] * P[(k - 1) * W + (m - i)];
            P[k * W + m] = s;
        }
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        for (size_t k = 2; k <= m; ++k) {
            seq_t t = b[k] * P[k * W + m];
            if (k & 1) neg += t; else pos += t;
        }
        av[m] = pos - neg;
        P[1 * W + m] = av[m];
    }

    if (A231690_cache) mem_free(A231690_cache);
    A231690_cache = av;
    A231690_cnt = N;
    mem_free(b);
    mem_free(P);
}

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;
    size_t m = (size_t)n;
    if (m > A231690_cnt) {
        size_t N = m;
        if (N < 2 * A231690_cnt) N = 2 * A231690_cnt;
        if (N < 32) N = 32;
        A231690_build(N);
    }
    return A231690_cache[m];
}
