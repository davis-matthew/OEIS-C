/*
 * A071644  a(n) = A005148(2^n-1)/8^(n-1).
 * https://oeis.org/A071644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A005148(2^n-1)/8^(n-1), where A005148(m) =
 *            (1/24)*binomial(2m,m)*(16^m - binomial(2m,m)^2) - Sum_{i=0..m-1}
 *            binomial(2m-2i,m-i)^3 * A005148(i), A005148(0) = 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

typedef unsigned __int128 A071644_u128;

static bool A071644_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

/* A005148(K) modulo 2^128 (exact while it fits) */
static A071644_u128 A071644_fixed_calc(idx_t K)
{
    A071644_u128 *av = (A071644_u128 *)malloc((size_t)(K + 1) * sizeof(A071644_u128));
    A071644_u128 *cb = (A071644_u128 *)malloc((size_t)(K + 1) * sizeof(A071644_u128));
    A071644_u128 *c3 = (A071644_u128 *)malloc((size_t)(K + 1) * sizeof(A071644_u128));
    A071644_u128 x = 3, p4 = 1, res;
    idx_t k, i;
    int t;
    for (t = 0; t < 8; t++)
        x = x * (2 - 3 * x);
    av[0] = 0; cb[0] = 1; c3[0] = 1;
    for (k = 1; k <= K; k++) {
        A071644_u128 h, f1, f2, e, s;
        cb[k] = cb[k - 1] * (A071644_u128)(4 * k - 2) / (A071644_u128)k;
        c3[k] = cb[k] * cb[k] * cb[k];
        p4 = p4 * 4;
        h = cb[k] / 2;
        f1 = (p4 - cb[k]) / 2;
        f2 = (p4 + cb[k]) / 2;
        e = h * f1 * f2 * x;
        s = 0;
        for (i = 1; i < k; i++)
            s += c3[k - i] * av[i];
        av[k] = e - s;
    }
    res = av[K];
    free(av); free(cb); free(c3);
    return res;
}

static seq_t A071644_big_calc(idx_t K)
{
    seq_t *av = mem_new(seq_t, (size_t)(K + 1));
    seq_t *cb = mem_new(seq_t, (size_t)(K + 1));
    seq_t *c3 = mem_new(seq_t, (size_t)(K + 1));
    seq_t p4 = 1, res;
    idx_t k, i;
    av[0] = 0; cb[0] = 1; c3[0] = 1;
    for (k = 1; k <= K; k++) {
        seq_t h, f1, f2, e, s;
        cb[k] = cb[k - 1] * (seq_t)(4 * k - 2) / (seq_t)k;
        c3[k] = cb[k] * cb[k] * cb[k];
        p4 = p4 * 4;
        h = cb[k] / 2;
        f1 = (p4 - cb[k]) / 2;
        f2 = (p4 + cb[k]) / 2;
        e = h * f1 * f2 / 3;
        s = 0;
        for (i = 1; i < k; i++)
            s = s + c3[k - i] * av[i];
        av[k] = e - s;
    }
    res = av[K];
    mem_free(av); mem_free(cb); mem_free(c3);
    return res;
}

seq_t a(idx_t n)
{
    unsigned sh = (unsigned)(3 * (n - 1));
    if (A071644_fixed_width()) {
        A071644_u128 v;
        seq_t r = 0;
        int i;
        if (n >= 40) return (seq_t)0;
        v = A071644_fixed_calc(((idx_t)1 << n) - 1) >> sh;
        for (i = 15; i >= 0; i--)
            r = (seq_t)((r << 8) | (seq_t)(unsigned)((v >> (8 * i)) & 0xff));
        return r;
    } else {
        seq_t v = A071644_big_calc(((idx_t)1 << n) - 1);
        return (seq_t)(v >> sh);
    }
}
