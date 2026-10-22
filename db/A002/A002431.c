/*
 * A002431  Numerators in Taylor series for cot x.
 * https://oeis.org/A002431
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, frac, easy, nice
 * Formula:   a(-1) = 1; for n >= 0, with m = n+1: a(n) =
 *            -numerator(4^m*|B_{2m}|/(2m)!), where |B_{2m}| =
 *            2m*T(m)/(4^m*(4^m-1)), T(m) = A000182(m) the tangent numbers
 *            (Brent-Harvey recurrence). Equivalently, with N =
 *            numerator(|B_{2m}|) = 2m*D*T(m)/(4^m*(4^m-1)), D =
 *            denominator(B_{2m}): |a(n)| = 2^(popcount(...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   a2431_blen, a2431_div, a2431_isprime, a2431_mod, a2431_mul,
 *            a2431_shadd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2431_blen.h"
#include "../utils/a2431_div.h"
#include "../utils/a2431_isprime.h"
#include "../utils/a2431_mod.h"
#include "../utils/a2431_mul.h"
#include "../utils/a2431_shadd.h"

static uint32_t *a2431_buf = NULL;
static size_t *a2431_off = NULL;
static size_t *a2431_len = NULL;
static idx_t a2431_M = 0;

static void a2431_build(idx_t M) {
    free(a2431_buf); free(a2431_off); free(a2431_len);
    a2431_off = (size_t *)malloc((size_t)(M + 2) * sizeof(size_t));
    a2431_len = (size_t *)malloc((size_t)(M + 2) * sizeof(size_t));
    size_t *cap = (size_t *)malloc((size_t)(M + 2) * sizeof(size_t));
    uint64_t lg = 0;
    size_t tot = 0;
    for (idx_t j = 1; j <= M; j++) {
        lg += (uint64_t)a2431_blen((uint64_t)(2 * j - 1)) + (uint64_t)a2431_blen((uint64_t)(2 * j));
        cap[j] = (size_t)(lg / 32 + 3);
        a2431_off[j] = tot;
        tot += cap[j];
    }
    a2431_buf = (uint32_t *)calloc(tot, sizeof(uint32_t));
    uint32_t *T1 = a2431_buf + a2431_off[1];
    T1[0] = 1;
    a2431_len[1] = 1;
    for (idx_t k = 2; k <= M; k++) {
        uint32_t *src = a2431_buf + a2431_off[k - 1];
        uint32_t *dst = a2431_buf + a2431_off[k];
        size_t L = a2431_len[k - 1];
        uint64_t c = 0;
        for (size_t i = 0; i < L; i++) {
            c += (uint64_t)src[i] * (uint64_t)(k - 1);
            dst[i] = (uint32_t)c;
            c >>= 32;
        }
        while (c) { dst[L++] = (uint32_t)c; c >>= 32; }
        a2431_len[k] = L;
    }
    for (idx_t k = 2; k <= M; k++) {
        for (idx_t j = k; j <= M; j++) {
            uint32_t *P = a2431_buf + a2431_off[j - 1];
            uint32_t *Q = a2431_buf + a2431_off[j];
            size_t lp = a2431_len[j - 1], lq = a2431_len[j];
            size_t L = lp > lq ? lp : lq;
            uint64_t aa = (uint64_t)(j - k), bb = (uint64_t)(j - k + 2);
            uint64_t c = 0;
            for (size_t i = 0; i < L; i++) {
                uint64_t xp = i < lp ? P[i] : 0;
                uint64_t xq = i < lq ? Q[i] : 0;
                c += xq * bb + xp * aa;
                Q[i] = (uint32_t)c;
                c >>= 32;
            }
            while (c) { Q[L++] = (uint32_t)c; c >>= 32; }
            a2431_len[j] = L;
        }
    }
    free(cap);
    a2431_M = M;
}

seq_t a(idx_t n) {
    if (n < 0) return 1;
    idx_t m = n + 1;
    if (m > a2431_M) {
        idx_t nm = m;
        if (nm < 2 * a2431_M) nm = 2 * a2431_M;
        if (nm < 32) nm = 32;
        a2431_build(nm);
    }
    uint64_t k = (uint64_t)(2 * m);

    /* primes of the Bernoulli denominator */
    size_t np = 0;
    uint32_t *pr = (uint32_t *)malloc((size_t)(k + 1) * sizeof(uint32_t));
    for (uint64_t d = 1; d <= k; d++)
        if (k % d == 0 && a2431_isprime(d + 1)) pr[np++] = (uint32_t)(d + 1);

    size_t W = a2431_len[m] + 3 + np;
    uint32_t *X = (uint32_t *)calloc(W, sizeof(uint32_t));
    memcpy(X, a2431_buf + a2431_off[m], a2431_len[m] * sizeof(uint32_t));
    size_t L = a2431_len[m];
    a2431_mul(X, &L, (uint32_t)k);
    for (size_t i = 0; i < np; i++) a2431_mul(X, &L, pr[i]);
    free(pr);

    /* divide by 4^m = 2^k */
    {
        size_t q = (size_t)(k / 32);
        unsigned r = (unsigned)(k % 32);
        for (size_t i = 0; i < W; i++) {
            uint32_t v = 0;
            if (i + q < W) {
                v = X[i + q] >> r;
                if (r && i + q + 1 < W) v |= X[i + q + 1] << (32 - r);
            }
            X[i] = v;
        }
    }
    size_t Lx = W;
    while (Lx > 0 && X[Lx - 1] == 0) Lx--;
    if (Lx == 0) { free(X); return 0; }

    /* exact division by 2^k - 1 via 2-adic inverse */
    uint32_t *S = (uint32_t *)calloc(Lx + 2, sizeof(uint32_t));
    for (uint64_t s = 0; s < 32 * (uint64_t)Lx; s += k) a2431_shadd(S, X, Lx, s);
    {
        uint64_t c = 1;
        for (size_t i = 0; i < Lx; i++) {
            c += (uint64_t)(uint32_t)~S[i];
            S[i] = (uint32_t)c;
            c >>= 32;
        }
    }
    free(X);
    size_t LN = Lx;
    while (LN > 0 && S[LN - 1] == 0) LN--;

    /* cancel odd primes shared with (2m)! */
    for (uint64_t p = 3; p <= k; p += 2) {
        if (!a2431_isprime(p)) continue;
        uint64_t e = 0, t = k;
        while (t) { t /= p; e += t; }
        while (e > 0 && LN > 0 && a2431_mod(S, LN, (uint32_t)p) == 0) {
            a2431_div(S, &LN, (uint32_t)p);
            e--;
        }
    }

    /* times 2^(popcount(m)-1) */
    int pc = 0;
    for (uint64_t t = (uint64_t)m; t; t >>= 1) pc += (int)(t & 1);
    for (int i = 1; i < pc; i++) a2431_mul(S, &LN, 2);

    seq_t val = 0;
    for (size_t i = LN; i-- > 0;) {
        val = val * 65536 + (seq_t)(int)((S[i] >> 16) & 0xFFFF);
        val = val * 65536 + (seq_t)(int)(S[i] & 0xFFFF);
    }
    free(S);
    return -val;
}
