/*
 * A007244  McKay-Thompson series of class 3B for the Monster group.
 * https://oeis.org/A007244
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f.: 12 + (eta(q)/eta(q^3))^12, i.e. a(n) = f(n+1) + 12*[n=0]
 *            where f = prod_{k>=1, 3 not| k} (1-q^k)^12 is computed by the
 *            log-derivative recurrence f(0) = 1, m*f(m) = Sum_{k=1..m}
 *            c(k)*f(m-k), c(k) = -12*(sigma(k) - 3*sigma(k/3))
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 33, i64 n <= 129, i128 n <= 453,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 904 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
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

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static idx_t *C = NULL;
static seq_t *F = NULL;
static __int128 *G = NULL;
static idx_t len = 0;

static void ensure(idx_t need, bool fixed) {
    if (need < len) return;
    idx_t nl = len ? len : 64;
    while (nl <= need) nl *= 2;
    idx_t *s = (idx_t *)calloc((size_t)nl, sizeof(idx_t));
    for (idx_t d = 1; d < nl; d++)
        for (idx_t m = d; m < nl; m += d) s[m] += d;
    free(C);
    C = (idx_t *)malloc((size_t)nl * sizeof(idx_t));
    C[0] = 0;
    for (idx_t k = 1; k < nl; k++)
        C[k] = -12 * (s[k] - (k % 3 == 0 ? 3 * s[k / 3] : 0));
    free(s);
    if (fixed) {
        __int128 *g = (__int128 *)malloc((size_t)nl * sizeof(__int128));
        for (idx_t i = 0; i < len; i++) g[i] = G[i];
        free(G);
        G = g;
        if (len == 0) G[0] = 1;
        for (idx_t m = (len ? len : 1); m < nl; m++) {
            __int128 sum = 0;
            for (idx_t k = 1; k <= m; k++) sum += (__int128)C[k] * G[m - k];
            G[m] = sum / m;
        }
    } else {
        seq_t *f = mem_new(seq_t, (size_t)nl);
        for (idx_t i = 0; i < len; i++) f[i] = F[i];
        mem_free(F);
        F = f;
        if (len == 0) F[0] = 1;
        for (idx_t m = (len ? len : 1); m < nl; m++) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; k++) sum += F[m - k] * C[k];
            F[m] = sum / m;
        }
    }
    len = nl;
}

seq_t a(idx_t n) {
    if (n < -1) return 0;
    bool fixed = seq_is_fixed_width();
    ensure(n + 1, fixed);
    if (fixed) {
        __int128 v = G[n + 1] + (n == 0 ? 12 : 0);
        return from_i128(v);
    }
    seq_t v = F[n + 1];
    if (n == 0) v += 12;
    return v;
}
