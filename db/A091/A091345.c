/*
 * A091345  Exponential convolution of A069321(n) with itself, where we set
 *          A069321(0)=0.
 * https://oeis.org/A091345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*c(k)*c(n-k), c(0)=0, c(k) =
 *            Sum_{i=1..k} i!*i*Stirling2(k,i) (A069321) for k>=1; e.g.f.
 *            (exp(x)-1)^2/(2-exp(x))^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *S = mem_new(seq_t, N + 2);
    seq_t *c = mem_new(seq_t, N + 2);
    seq_t *F = mem_new(seq_t, N + 2);   /* factorials */
    seq_t *Bn = mem_new(seq_t, N + 2);  /* Pascal row of binomials */

    F[0] = (seq_t)1;
    for (size_t k = 1; k <= N; ++k)
        F[k] = F[k - 1] * (seq_t)k;

    S[0] = (seq_t)1;
    c[0] = (seq_t)0;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k)
            S[k] = (seq_t)k * S[k] + S[k - 1];
        S[0] = (seq_t)0;
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= m; ++k)
            sum += S[k] * (seq_t)k * F[k];
        c[m] = sum;
    }

    /* Pascal row n */
    Bn[0] = (seq_t)1;
    for (size_t r = 1; r <= N; ++r) {
        for (size_t k = r; k >= 1; --k)
            Bn[k] = Bn[k] + Bn[k - 1];
    }

    seq_t res = (seq_t)0;
    for (size_t k = 0; k <= N; ++k)
        res += Bn[k] * c[k] * c[N - k];

    mem_free(S);
    mem_free(c);
    mem_free(F);
    mem_free(Bn);
    return res;
}
