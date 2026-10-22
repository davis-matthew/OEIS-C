/*
 * A005482  Decimal expansion of cube root of 7.
 * https://oeis.org/A005482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit of 7^(1/3) = 1.9129311827..., found by
 *            digit-by-digit cube root extraction: r = 1000*r + group, choose
 *            the largest d with (300*y^2 + 30*y*d + d^2)*d <= r
 * Range:     u8 n <= 9066, u16 n <= 9066, u32 n <= 9066, u64 n <= 9066,
 *            u128 n <= 9066, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BN_BASE 1000000000u

typedef struct { uint32_t *v; size_t n, cap; } bn;

static void bn_res(bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 8;
        x->v = (uint32_t *)realloc(x->v, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void bn_trim(bn *x) {
    while (x->n && x->v[x->n - 1] == 0) x->n--;
}

static void bn_muls(bn *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->n = 0; return; }
    for (i = 0; i < x->n; i++) {
        c += (uint64_t)x->v[i] * m;
        x->v[i] = (uint32_t)(c % BN_BASE);
        c /= BN_BASE;
    }
    while (c) {
        bn_res(x, x->n + 1);
        x->v[x->n++] = (uint32_t)(c % BN_BASE);
        c /= BN_BASE;
    }
}

static void bn_adds(bn *x, uint32_t s) {
    uint64_t c = s;
    size_t i = 0;
    while (c) {
        if (i == x->n) { bn_res(x, x->n + 1); x->v[x->n++] = 0; }
        c += x->v[i];
        x->v[i] = (uint32_t)(c % BN_BASE);
        c /= BN_BASE;
        i++;
    }
}

static void bn_add(bn *x, const bn *y) {
    size_t i, m = x->n > y->n ? x->n : y->n;
    uint64_t c = 0;
    bn_res(x, m + 1);
    for (i = x->n; i < m; i++) x->v[i] = 0;
    x->n = m;
    for (i = 0; i < m; i++) {
        c += (uint64_t)x->v[i] + (i < y->n ? y->v[i] : 0);
        x->v[i] = (uint32_t)(c % BN_BASE);
        c /= BN_BASE;
    }
    if (c) x->v[x->n++] = (uint32_t)c;
}

/* x -= y, requires x >= y */
static void bn_sub(bn *x, const bn *y) {
    size_t i;
    int64_t b = 0;
    for (i = 0; i < x->n; i++) {
        int64_t t = (int64_t)x->v[i] - b - (i < y->n ? (int64_t)y->v[i] : 0);
        if (t < 0) { t += BN_BASE; b = 1; } else b = 0;
        x->v[i] = (uint32_t)t;
    }
    bn_trim(x);
}

static int bn_cmp(const bn *x, const bn *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i > 0; i--) {
        if (x->v[i - 1] != y->v[i - 1]) return x->v[i - 1] < y->v[i - 1] ? -1 : 1;
    }
    return 0;
}

static void bn_copy(bn *d, const bn *s) {
    bn_res(d, s->n + 1);
    if (s->n) memcpy(d->v, s->v, s->n * sizeof(uint32_t));
    d->n = s->n;
}

static bn st_r, st_y, st_y2, st_A, st_B, st_T, st_tmp;
static unsigned char *st_dig = NULL;
static size_t st_nd = 0, st_dcap = 0;

/* T = d*(A + d*(B + d)) */
static void bn_trial(uint32_t d) {
    bn_copy(&st_T, &st_B);
    bn_adds(&st_T, d);
    bn_muls(&st_T, d);
    bn_add(&st_T, &st_A);
    bn_muls(&st_T, d);
}

static void extend_one(void) {
    uint32_t g = (st_nd == 0) ? 7u : 0u;
    uint32_t lo = 0, hi = 9, d;

    bn_muls(&st_r, 1000);
    bn_adds(&st_r, g);

    bn_copy(&st_A, &st_y2);
    bn_muls(&st_A, 300);
    bn_copy(&st_B, &st_y);
    bn_muls(&st_B, 30);

    while (lo < hi) {
        uint32_t mid = (lo + hi + 1) / 2;
        bn_trial(mid);
        if (bn_cmp(&st_T, &st_r) <= 0) lo = mid; else hi = mid - 1;
    }
    d = lo;
    bn_trial(d);
    bn_sub(&st_r, &st_T);

    /* y2 = 100*y2 + 20*y*d + d^2 */
    bn_muls(&st_y2, 100);
    bn_copy(&st_tmp, &st_y);
    bn_muls(&st_tmp, 20 * d);
    bn_add(&st_y2, &st_tmp);
    bn_adds(&st_y2, d * d);

    /* y = 10*y + d */
    bn_muls(&st_y, 10);
    bn_adds(&st_y, d);

    if (st_nd == st_dcap) {
        st_dcap = st_dcap * 2 + 64;
        st_dig = (unsigned char *)realloc(st_dig, st_dcap);
    }
    st_dig[st_nd++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while ((idx_t)st_nd < n) extend_one();
    return (seq_t)st_dig[n - 1];
}
