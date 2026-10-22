/*
 * A003303  Numerators of spin-wave coefficients for cubic lattice.
 * https://oeis.org/A003303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   256*(n+1)*g(n+1) = 32*(22*n^2+22*n+9)*g(n) -
 *            144*n*(4*n^2+1)*g(n-1) + 9*(2*n-1)^4*g(n-2), g(-2) = g(-1) = 0,
 *            g(0) = 1; a(n) = numerator(g(n)). g(n) is kept as U/2^E with
 *            exact multi-limb integers; the odd part of (n+1) divides exactly
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t len, cap; } s303_bn;

static void s303_reserve(s303_bn *x, size_t c) {
    if (x->cap < c) {
        size_t nc = c * 2 + 8;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void s303_trim(s303_bn *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

static void s303_set(s303_bn *x, uint32_t v) {
    s303_reserve(x, 1);
    x->d[0] = v;
    x->len = v ? 1 : 0;
}

static void s303_copy(s303_bn *dst, const s303_bn *src) {
    s303_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void s303_mul(s303_bn *x, uint64_t m) {
    unsigned __int128 c = 0;
    size_t i;
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) {
        unsigned __int128 t = (unsigned __int128)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        s303_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
        c >>= 32;
    }
}

static void s303_add(s303_bn *x, const s303_bn *y) {
    size_t n = x->len > y->len ? x->len : y->len, i;
    uint64_t c = 0;
    s303_reserve(x, n + 1);
    for (i = x->len; i < n; i++) x->d[i] = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)x->d[i] + (i < y->len ? y->d[i] : 0) + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x->d[n++] = (uint32_t)c;
    x->len = n;
}

/* x -= y, assumes x >= y */
static void s303_sub(s303_bn *x, const s303_bn *y) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < x->len; i++) {
        int64_t t = (int64_t)x->d[i] - (int64_t)(i < y->len ? y->d[i] : 0) - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)t;
    }
    s303_trim(x);
}

static void s303_div(s303_bn *x, uint32_t m) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    s303_trim(x);
}

static uint64_t s303_tz(const s303_bn *x) {
    size_t i = 0;
    uint32_t v;
    uint64_t r;
    if (x->len == 0) return UINT64_MAX;
    while (x->d[i] == 0) i++;
    v = x->d[i];
    r = (uint64_t)i * 32;
    while (!(v & 1)) { v >>= 1; r++; }
    return r;
}

static void s303_shl(s303_bn *x, uint64_t s) {
    size_t w = (size_t)(s / 32), n = x->len, i;
    unsigned b = (unsigned)(s % 32);
    if (n == 0 || s == 0) return;
    s303_reserve(x, n + w + 1);
    x->d[n + w] = 0;
    i = n;
    while (i-- > 0) {
        uint32_t v = x->d[i];
        if (b) {
            x->d[i + w + 1] |= v >> (32 - b);
            x->d[i + w] = v << b;
        } else {
            x->d[i + w] = v;
        }
    }
    for (i = 0; i < w; i++) x->d[i] = 0;
    x->len = n + w + 1;
    s303_trim(x);
}

static void s303_shr(s303_bn *x, uint64_t s) {
    size_t w = (size_t)(s / 32), i;
    unsigned b = (unsigned)(s % 32);
    if (s == 0 || x->len == 0) return;
    if (w >= x->len) { x->len = 0; return; }
    for (i = 0; i + w < x->len; i++) {
        uint32_t lo = x->d[i + w] >> b;
        if (b && i + w + 1 < x->len) lo |= x->d[i + w + 1] << (32 - b);
        x->d[i] = lo;
    }
    x->len -= w;
    s303_trim(x);
}

static s303_bn s303_U[3], s303_X, s303_T;
static idx_t s303_k = 0;
static uint64_t s303_E = 0;
static int s303_init = 0;

static void s303_reset(void) {
    s303_U[0].len = 0;
    s303_U[1].len = 0;
    s303_set(&s303_U[2], 1);
    s303_E = 0;
    s303_k = 0;
    s303_init = 1;
}

static void s303_step(void) {
    uint64_t nn = (uint64_t)s303_k;
    uint64_t c1 = 22 * nn * nn + 22 * nn + 9;
    uint64_t c2 = 4 * nn * nn + 1;
    uint64_t d = nn ? 2 * nn - 1 : 1;
    uint64_t d2 = d * d;
    uint64_t k1 = nn + 1, m = k1, t = 0, z, tz;
    s303_bn tmp;
    int i;

    s303_copy(&s303_X, &s303_U[2]);
    s303_mul(&s303_X, 32 * c1);
    s303_copy(&s303_T, &s303_U[0]);
    s303_mul(&s303_T, d2);
    s303_mul(&s303_T, 9 * d2);
    s303_add(&s303_X, &s303_T);
    s303_copy(&s303_T, &s303_U[1]);
    s303_mul(&s303_T, 144 * nn * c2);
    s303_sub(&s303_X, &s303_T);

    while (!(m & 1)) { m >>= 1; t++; }
    if (m > 1) s303_div(&s303_X, (uint32_t)m);

    s303_shl(&s303_U[1], 8 + t);
    s303_shl(&s303_U[2], 8 + t);
    s303_E += 8 + t;

    tmp = s303_U[0];
    s303_U[0] = s303_U[1];
    s303_U[1] = s303_U[2];
    s303_U[2] = s303_X;
    s303_X = tmp;

    z = s303_E;
    for (i = 0; i < 3; i++) {
        tz = s303_tz(&s303_U[i]);
        if (tz < z) z = tz;
    }
    if (z) {
        for (i = 0; i < 3; i++) s303_shr(&s303_U[i], z);
        s303_E -= z;
    }
    s303_k++;
}

seq_t a(idx_t n) {
    seq_t res = 0, probe = 1;
    uint64_t z;
    size_t i, top;
    int fixed, j;

    if (!s303_init || n < s303_k) s303_reset();
    while (s303_k < n) s303_step();

    s303_copy(&s303_T, &s303_U[2]);
    z = s303_tz(&s303_T);
    if (z > s303_E) z = s303_E;
    s303_shr(&s303_T, z);

    for (j = 0; j < 200; j++) probe = (seq_t)(probe + probe);
    fixed = (probe == 0);

    top = s303_T.len;
    if (fixed) {
        if (top > 4) top = 4;
        i = top;
        while (i-- > 0) {
            for (j = 0; j < 32; j++) res = (seq_t)(res + res);
            res = (seq_t)(res + (seq_t)s303_T.d[i]);
        }
    } else {
        i = top;
        while (i-- > 0) {
            res <<= 32;
            res += (seq_t)s303_T.d[i];
        }
    }
    return res;
}
