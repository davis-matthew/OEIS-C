/*
 * A179488  G.f.: A(x) satisfies A(x) = x/(1 - (1-2x)*A( x/(1-2x) )).
 * https://oeis.org/A179488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>1: Let ay(t) = Σ_{k=1}^{t} a(k) * C(t‑1, k‑1) *
 *            2^{t‑k}. Let conv(n) = Σ_{i=1}^{n‑1} a(i) * ay(n‑i). Then a(n) =
 *            conv(n) – 2·conv(n‑1). The values are derived from the
 *            functional equation A(x) = x / (1 – (1‑2x)·A(x/(1‑2x)))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for a(1..n) and ay(1..n‑1) */
    size_t sz = (size_t)(n + 1);
    seq_t *A  = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *AY = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!A || !AY) return 0;  /* out‑of‑memory protection */

    /* initialise to zero */
    for (idx_t i = 0; i < sz; ++i) {
        A[i]  = 0;
        AY[i] = 0;
    }

    if (n == 0) {
        free(A); free(AY);
        return 0;
    }

    /* a(1) = 1 */
    A[1] = 1;

    /* Update AY with contributions from a(1) */
    if (n > 1) {
        seq_t w = 1;
        for (idx_t t = 1; t <= n-1; ++t) {
            AY[t] += (seq_t)A[1] * w;
            if (t == n-1) break;
            /* w = w * 2 * t / (t)  (since k=1) */
            w = w * (seq_t)2 * (seq_t)t / (seq_t)((t - 1 + 1));
        }
    }

    seq_t conv_prev = 0;   /* conv(1) = 0 */

    for (idx_t m = 2; m <= n; ++m) {
        /* compute conv(m) = Σ_{i=1}^{m‑1} a(i) * ay(m‑i) */
        seq_t conv_m = 0;
        for (idx_t i = 1; i <= m-1; ++i) {
            conv_m += A[i] * AY[m-i];
        }

        /* a(m) = conv(m) – 2·conv(m‑1) */
        A[m] = conv_m - conv_prev * (seq_t)2;

        /* update AY for indices ≥ m up to n‑1 */
        if (m <= n-1) {
            seq_t w = 1;
            for (idx_t t = m; t <= n-1; ++t) {
                AY[t] += A[m] * w;
                if (t == n-1) break;
                /* w = w * 2 * t / (t‑m+1) */
                w = w * (seq_t)2 * (seq_t)t / (seq_t)((t - m + 1));
            }
        }

        conv_prev = conv_m;
    }

    seq_t result = A[n];
    free(A); free(AY);
    return result;
}
