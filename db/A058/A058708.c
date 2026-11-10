/*
 * A058708  McKay-Thompson series of class 54A for the Monster group.
 * https://oeis.org/A058708
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n+1)] A(x)*B(x) - [n==0], with A = G(x^54)*G(x) +
 *            x^11*H(x^54)*H(x), B = G(x^27)*H(x^2) - x^5*H(x^27)*G(x^2), G =
 *            g.f. of A003114, H = g.f. of A003106
 * Range:     u8 n <= 30, u16 n <= 85, u32 n <= 262, u64 n <= 882,
 *            u128 n <= 3158, big any n
 * Validated: standard, pass, 73 terms through n = 8276 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A058708_gh(idx_t N, seq_t *g, seq_t *h) {
    idx_t part, i;
    g[0] = 1;
    h[0] = 1;
    for (part = 1; part <= N; ++part) {
        int r = (int)(part % 5);
        if (r == 1 || r == 4) {
            for (i = part; i <= N; ++i) g[i] += g[i - part];
        } else if (r == 2 || r == 3) {
            for (i = part; i <= N; ++i) h[i] += h[i - part];
        }
    }
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    idx_t i, k;
    if (N < 0) return 0;
    seq_t *g = mem_new(seq_t, (size_t)(N + 1));
    seq_t *h = mem_new(seq_t, (size_t)(N + 1));
    seq_t *A = mem_new(seq_t, (size_t)(N + 1));
    seq_t *Bp = mem_new(seq_t, (size_t)(N + 1));
    seq_t *Bn = mem_new(seq_t, (size_t)(N + 1));
    A058708_gh(N, g, h);

    for (i = 0; i <= N; ++i) {
        seq_t s = 0;
        for (k = 0; 54 * k <= i; ++k) s += g[k] * g[i - 54 * k];
        for (k = 0; 11 + 54 * k <= i; ++k) s += h[k] * h[i - 11 - 54 * k];
        A[i] = s;
        seq_t p = 0;
        for (k = 0; 27 * k <= i; ++k) {
            idx_t m = i - 27 * k;
            if ((m & 1) == 0) p += g[k] * h[m / 2];
        }
        Bp[i] = p;
        seq_t q = 0;
        for (k = 0; 5 + 27 * k <= i; ++k) {
            idx_t m = i - 5 - 27 * k;
            if ((m & 1) == 0) q += h[k] * g[m / 2];
        }
        Bn[i] = q;
    }

    seq_t pos = 0, neg = 0;
    for (i = 0; i <= N; ++i) {
        pos += A[i] * Bp[N - i];
        neg += A[i] * Bn[N - i];
    }
    seq_t res = pos - neg;
    if (N == 1) res -= 1;

    mem_free(g);
    mem_free(h);
    mem_free(A);
    mem_free(Bp);
    mem_free(Bn);
    return res;
}
