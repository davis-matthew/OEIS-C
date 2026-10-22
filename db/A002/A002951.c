/*
 * A002951  Continued fraction for fifth root of 5.
 * https://oeis.org/A002951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction expansion of 5^(1/5); y = 5^(-1/5) by
 *            Newton's method in fixed point (y <- y + y*(1-5*y^5)/5), x =
 *            5*y^4, bracket checked exactly as A^5 < 5*D^5 < B^5, terms kept
 *            only where the expansions of A/D and B/D agree
 * Range:     u8 n <= 69, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   bn_bitlen, bn_bits, bn_cmp, bn_mul, bn_norm, bn_submul, cf_step,
 *            pow5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bn_bitlen.h"
#include "../utils/bn_bits.h"
#include "../utils/bn_cmp.h"
#include "../utils/bn_mul.h"
#include "../utils/bn_norm.h"
#include "../utils/bn_submul.h"
#include "../utils/cf_step.h"
#include "../utils/pow5.h"

static uint64_t *cf_terms = NULL;
static idx_t cf_nterms = 0;

static void bn_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bn_sub(uint32_t *a, const uint32_t *b, size_t L) {
    uint32_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t d = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)d;
        br = (uint32_t)((d >> 32) & 1);
    }
}

static void bn_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bn_div_small(uint32_t *x, size_t L, uint32_t d) {
    uint64_t rem = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void fx_mul(uint32_t *r, const uint32_t *u, const uint32_t *v, size_t L, size_t F, uint32_t *tmp) {
    bn_mul(tmp, u, L, v, L);
    memcpy(r, tmp + F, L * sizeof(uint32_t));
}

static size_t cf_compute(size_t F, uint64_t *out, size_t cap) {
    size_t L = F + 1, i, count = 0, k;
    int it, iters = 2;
    size_t bits = 4;
    int ok = 0;
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y4 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *one = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *qa = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *qb = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *C = (uint32_t *)calloc(5 * L, sizeof(uint32_t));
    uint32_t *P5 = (uint32_t *)calloc(5 * L, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint32_t *t4 = (uint32_t *)calloc(4 * L, sizeof(uint32_t));

    one[F] = 1;
    y[F - 1] = 0xC0000000u; /* 0.75 */
    while (bits < 32 * F + 64) { bits *= 2; iters++; }
    for (it = 0; it < iters; it++) {
        fx_mul(y2, y, y, L, F, tmp);
        fx_mul(y4, y2, y2, L, F, tmp);
        fx_mul(t, y4, y, L, F, tmp);
        bn_mul_small(t, L, 5);
        if (bn_cmp(t, L, one, L) >= 0) {
            memcpy(d, t, L * sizeof(uint32_t));
            bn_sub(d, one, L);
            fx_mul(w, y, d, L, F, tmp);
            bn_div_small(w, L, 5);
            bn_sub(y, w, L);
        } else {
            memcpy(d, one, L * sizeof(uint32_t));
            bn_sub(d, t, L);
            fx_mul(w, y, d, L, F, tmp);
            bn_div_small(w, L, 5);
            bn_add(y, w, L);
        }
    }
    fx_mul(y2, y, y, L, F, tmp);
    fx_mul(y4, y2, y2, L, F, tmp);
    memcpy(t, y4, L * sizeof(uint32_t));
    bn_mul_small(t, L, 5); /* t ~ 5^(1/5) * 2^(32F) */

    C[5 * F] = 5;
    for (k = 1; k + 2 < F; k++) {
        memset(d, 0, L * sizeof(uint32_t));
        d[k] = 1;
        memcpy(A, t, L * sizeof(uint32_t));
        bn_sub(A, d, L);
        memcpy(B, t, L * sizeof(uint32_t));
        bn_add(B, d, L);
        pow5(A, L, P5, t2, t4);
        if (bn_cmp(P5, 5 * L, C, 5 * L) >= 0) continue;
        pow5(B, L, P5, t2, t4);
        if (bn_cmp(P5, 5 * L, C, 5 * L) <= 0) continue;
        ok = 1;
        break;
    }

    if (ok) {
        uint32_t *pa = A, *pb = B, *ra = qa, *rb = qb, *sw;
        size_t lpa, lpb, lqa, lqb, sl;
        memset(qa, 0, (L + 2) * sizeof(uint32_t));
        memset(qb, 0, (L + 2) * sizeof(uint32_t));
        qa[F] = 1;
        qb[F] = 1;
        lpa = bn_norm(pa, L);
        lpb = bn_norm(pb, L);
        lqa = bn_norm(ra, L);
        lqb = bn_norm(rb, L);
        while (count < cap && lqa && lqb) {
            uint64_t ta = cf_step(pa, &lpa, ra, lqa);
            uint64_t tb = cf_step(pb, &lpb, rb, lqb);
            if (ta != tb || lpa == 0 || lpb == 0) break;
            out[count++] = ta;
            sw = pa; pa = ra; ra = sw; sl = lpa; lpa = lqa; lqa = sl;
            sw = pb; pb = rb; rb = sw; sl = lpb; lpb = lqb; lqb = sl;
        }
    }

    free(y); free(y2); free(y4); free(t); free(d); free(w); free(one); free(tmp);
    free(A); free(B); free(qa); free(qb); free(C); free(P5); free(t2); free(t4);
    (void)i;
    return count;
}

seq_t a(idx_t n) {
    if (n >= cf_nterms) {
        idx_t target = n + 1;
        size_t F, cap, cnt;
        uint64_t *buf;
        if (target < 2 * cf_nterms) target = 2 * cf_nterms;
        if (target < 32) target = 32;
        F = (size_t)(target * 4 / 32) + 8;
        for (;;) {
            cap = 64 * F + 16;
            buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
            cnt = cf_compute(F, buf, cap);
            if (cnt > n) break;
            free(buf);
            F *= 2;
        }
        free(cf_terms);
        cf_terms = buf;
        cf_nterms = cnt;
    }
    return (seq_t)cf_terms[n];
}
