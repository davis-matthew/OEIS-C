/*
 * A002352  Numerators of convergents to cube root of 2.
 * https://oeis.org/A002352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = 1, where c(n) are
 *            the continued fraction partial quotients of 2^(1/3) =
 *            [1;3,1,5,1,1,4,...], computed exactly by Lagrange's method
 *            (repeated Taylor shift and inversion of x^3 - 2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   a2352_mag_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2352_mag_cmp.h"

typedef struct { uint32_t *d; size_t len, cap; int neg; } a2352_bn;

static void a2352_res(a2352_bn *x, size_t n) {
    if (n > x->cap) {
        size_t c = x->cap ? x->cap : 8;
        while (c < n) c *= 2;
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        x->cap = c;
    }
}

static void a2352_trim(a2352_bn *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
    if (!x->len) x->neg = 0;
}

static void a2352_set_small(a2352_bn *x, uint32_t v, int neg) {
    a2352_res(x, 1);
    x->d[0] = v; x->len = 1; x->neg = neg;
    a2352_trim(x);
}

static void a2352_copy(a2352_bn *x, const a2352_bn *y) {
    a2352_res(x, y->len + 1);
    if (y->len) memcpy(x->d, y->d, y->len * sizeof(uint32_t));
    x->len = y->len; x->neg = y->neg;
}

static void a2352_mul_small(a2352_bn *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->len = 0; x->neg = 0; return; }
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        a2352_res(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
    }
}

/* x += (neg ? -1 : 1) * d ; d must not alias x */
static void a2352_add_signed(a2352_bn *x, const uint32_t *d, size_t len, int neg) {
    size_t i;
    if (len == 0) return;
    if (x->len == 0 || x->neg == neg) {
        size_t n = (x->len > len ? x->len : len) + 1;
        uint64_t c = 0;
        a2352_res(x, n);
        for (i = x->len; i < n; i++) x->d[i] = 0;
        for (i = 0; i < n; i++) {
            uint64_t t = (uint64_t)x->d[i] + (i < len ? d[i] : 0) + c;
            x->d[i] = (uint32_t)t;
            c = t >> 32;
        }
        x->len = n;
        x->neg = neg;
        a2352_trim(x);
    } else if (a2352_mag_cmp(x->d, x->len, d, len) >= 0) {
        int64_t br = 0;
        for (i = 0; i < x->len; i++) {
            int64_t t = (int64_t)x->d[i] - (int64_t)(i < len ? d[i] : 0) - br;
            if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
            x->d[i] = (uint32_t)t;
        }
        a2352_trim(x);
    } else {
        int64_t br = 0;
        a2352_res(x, len);
        for (i = x->len; i < len; i++) x->d[i] = 0;
        for (i = 0; i < len; i++) {
            int64_t t = (int64_t)d[i] - (int64_t)x->d[i] - br;
            if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
            x->d[i] = (uint32_t)t;
        }
        x->len = len;
        x->neg = neg;
        a2352_trim(x);
    }
}

static a2352_bn a2352_P[4];
static a2352_bn a2352_tmp;
static a2352_bn a2352_ev;
static uint32_t *a2352_cf = NULL;
static size_t a2352_ncf = 0, a2352_capcf = 0;
static int a2352_init = 0;

static void a2352_addmul(a2352_bn *x, const a2352_bn *y, uint32_t b) {
    a2352_copy(&a2352_tmp, y);
    a2352_mul_small(&a2352_tmp, b);
    a2352_trim(&a2352_tmp);
    a2352_add_signed(x, a2352_tmp.d, a2352_tmp.len, a2352_tmp.neg);
}

static int a2352_sign_at(uint32_t t) {
    int i;
    a2352_copy(&a2352_ev, &a2352_P[3]);
    for (i = 2; i >= 0; i--) {
        a2352_mul_small(&a2352_ev, t);
        a2352_add_signed(&a2352_ev, a2352_P[i].d, a2352_P[i].len, a2352_P[i].neg);
    }
    if (a2352_ev.len == 0) return 0;
    return a2352_ev.neg ? -1 : 1;
}

static void a2352_next(void) {
    uint64_t lo = 1, hi = 2;
    uint32_t b;
    int k, i;
    a2352_bn t;
    while (a2352_sign_at((uint32_t)hi) < 0) {
        lo = hi;
        hi *= 2;
        if (hi > 0xFFFFFFFFull) { hi = 0xFFFFFFFFull; break; }
    }
    while (hi - lo > 1) {
        uint64_t mid = (lo + hi) / 2;
        if (a2352_sign_at((uint32_t)mid) < 0) lo = mid; else hi = mid;
    }
    b = (uint32_t)lo;
    /* Taylor shift: P(y) -> P(y + b) */
    for (k = 0; k < 3; k++)
        for (i = 2; i >= k; i--)
            a2352_addmul(&a2352_P[i], &a2352_P[i + 1], b);
    /* y^3 * P(b + 1/y), negated so the leading coefficient is positive */
    t = a2352_P[0]; a2352_P[0] = a2352_P[3]; a2352_P[3] = t;
    t = a2352_P[1]; a2352_P[1] = a2352_P[2]; a2352_P[2] = t;
    for (i = 0; i < 4; i++)
        if (a2352_P[i].len) a2352_P[i].neg ^= 1;
    if (a2352_ncf == a2352_capcf) {
        a2352_capcf = a2352_capcf ? a2352_capcf * 2 : 64;
        a2352_cf = (uint32_t *)realloc(a2352_cf, a2352_capcf * sizeof(uint32_t));
    }
    a2352_cf[a2352_ncf++] = b;
}

seq_t a(idx_t n) {
    seq_t pm, p;
    idx_t i;
    if (!a2352_init) {
        a2352_set_small(&a2352_P[3], 1, 0);
        a2352_set_small(&a2352_P[2], 0, 0);
        a2352_set_small(&a2352_P[1], 0, 0);
        a2352_set_small(&a2352_P[0], 2, 1);
        a2352_init = 1;
    }
    while ((idx_t)a2352_ncf <= n) a2352_next();
    pm = 1;
    p = (seq_t)a2352_cf[0];
    for (i = 1; i <= n; i++) {
        seq_t c = (seq_t)a2352_cf[i];
        seq_t nx = c * p + pm;
        pm = p;
        p = nx;
    }
    return p;
}
