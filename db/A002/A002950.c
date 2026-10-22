/*
 * A002950  Continued fraction for fifth root of 2.
 * https://oeis.org/A002950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = n-th partial quotient of the continued fraction of
 *            2^(1/5). Computed exactly with the polynomial (Vincent/Lagrange)
 *            method: start from P(x) = x^5 - 2; repeatedly a = floor(positive
 *            root of P) (found by sign tests of P at integers), then P(x) <-
 *            x^5 * P(a + 1/x)
 * Range:     u8 n <= 113, u16 n <= 9596, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   q2950_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q2950_cmp.h"

typedef struct { uint32_t *d; size_t n, cap; int neg; } q2950_bn;

static q2950_bn q2950_P[6];
static q2950_bn q2950_T, q2950_A, q2950_B;
static uint64_t *q2950_terms = NULL;
static size_t q2950_len = 0, q2950_cap = 0;
static int q2950_init = 0;

static void q2950_reserve(q2950_bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 8;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void q2950_trim(q2950_bn *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void q2950_addmag(q2950_bn *x, const uint32_t *y, size_t yn) {
    size_t m = x->n > yn ? x->n : yn;
    size_t i;
    uint64_t c = 0;
    q2950_reserve(x, m + 1);
    for (i = x->n; i < m + 1; i++) x->d[i] = 0;
    for (i = 0; i < m; i++) {
        c += (uint64_t)x->d[i] + (i < yn ? y[i] : 0);
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    x->d[m] = (uint32_t)c;
    x->n = m + 1;
    q2950_trim(x);
}

static void q2950_addsigned(q2950_bn *x, const uint32_t *y, size_t yn, int yneg) {
    size_t i;
    int64_t b = 0;
    int c;
    if (!yn) return;
    if (!x->n) {
        q2950_reserve(x, yn);
        memcpy(x->d, y, yn * sizeof(uint32_t));
        x->n = yn;
        x->neg = yneg;
        return;
    }
    if (x->neg == yneg) {
        q2950_addmag(x, y, yn);
        return;
    }
    c = q2950_cmp(x->d, x->n, y, yn);
    if (c == 0) { x->n = 0; x->neg = 0; return; }
    if (c > 0) {
        for (i = 0; i < x->n; i++) {
            int64_t t = (int64_t)x->d[i] - (int64_t)(i < yn ? y[i] : 0) - b;
            b = t < 0;
            if (t < 0) t += (int64_t)1 << 32;
            x->d[i] = (uint32_t)t;
        }
        q2950_trim(x);
    } else {
        q2950_reserve(x, yn);
        for (i = x->n; i < yn; i++) x->d[i] = 0;
        for (i = 0; i < yn; i++) {
            int64_t t = (int64_t)y[i] - (int64_t)x->d[i] - b;
            b = t < 0;
            if (t < 0) t += (int64_t)1 << 32;
            x->d[i] = (uint32_t)t;
        }
        x->n = yn;
        x->neg = yneg;
        q2950_trim(x);
    }
}

/* dst = src * m (dst may equal src) */
static void q2950_mulsmall(q2950_bn *dst, q2950_bn *src, uint32_t m) {
    size_t sn = src->n, i;
    uint64_t c = 0;
    int sg = src->neg;
    q2950_reserve(dst, sn + 1);
    for (i = 0; i < sn; i++) {
        c += (uint64_t)src->d[i] * m;
        dst->d[i] = (uint32_t)c;
        c >>= 32;
    }
    dst->d[sn] = (uint32_t)c;
    dst->n = sn + 1;
    dst->neg = sg;
    q2950_trim(dst);
}

/* sign of P(c) */
static int q2950_sign_at(uint32_t c) {
    int i;
    q2950_A.n = 0; q2950_A.neg = 0;
    q2950_B.n = 0; q2950_B.neg = 0;
    for (i = 5; i >= 0; i--) {
        q2950_mulsmall(&q2950_A, &q2950_A, c);
        q2950_mulsmall(&q2950_B, &q2950_B, c);
        if (q2950_P[i].n) {
            if (q2950_P[i].neg) q2950_addmag(&q2950_B, q2950_P[i].d, q2950_P[i].n);
            else q2950_addmag(&q2950_A, q2950_P[i].d, q2950_P[i].n);
        }
    }
    return q2950_cmp(q2950_A.d, q2950_A.n, q2950_B.d, q2950_B.n) > 0 ? 1 : -1;
}

static void q2950_shift(uint32_t t) {
    int i, j;
    for (i = 0; i < 5; i++)
        for (j = 4; j >= i; j--) {
            q2950_mulsmall(&q2950_T, &q2950_P[j + 1], t);
            q2950_addsigned(&q2950_P[j], q2950_T.d, q2950_T.n, q2950_T.neg);
        }
}

static uint64_t q2950_next(void) {
    int s = q2950_P[5].neg ? -1 : 1;
    uint64_t lo = 1, hi = 2;
    q2950_bn tmp;
    int i;
    while (hi < ((uint64_t)1 << 32) && q2950_sign_at((uint32_t)hi) == -s) {
        lo = hi;
        hi *= 2;
    }
    if (hi > 0xFFFFFFFFull) hi = 0xFFFFFFFFull;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (q2950_sign_at((uint32_t)mid) == -s) lo = mid;
        else hi = mid;
    }
    q2950_shift((uint32_t)lo);
    for (i = 0; i < 3; i++) {
        tmp = q2950_P[i];
        q2950_P[i] = q2950_P[5 - i];
        q2950_P[5 - i] = tmp;
    }
    return lo;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    if (!q2950_init) {
        q2950_reserve(&q2950_P[5], 1);
        q2950_P[5].d[0] = 1; q2950_P[5].n = 1; q2950_P[5].neg = 0;
        q2950_reserve(&q2950_P[0], 1);
        q2950_P[0].d[0] = 2; q2950_P[0].n = 1; q2950_P[0].neg = 1;
        q2950_init = 1;
    }
    while (q2950_len <= n) {
        if (q2950_len == q2950_cap) {
            q2950_cap = q2950_cap * 2 + 64;
            q2950_terms = (uint64_t *)realloc(q2950_terms, q2950_cap * sizeof(uint64_t));
        }
        q2950_terms[q2950_len] = q2950_next();
        q2950_len++;
    }
    r += (idx_t)q2950_terms[n];
    return r;
}
