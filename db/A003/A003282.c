/*
 * A003282  Numerators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   8*(n+1)*(2n+1)*(2n+3)*C(n+1) = 6*(2n+1)*(5n^2+5n+2)*C(n) -
 *            24*n^3*C(n-1) - 2*n*(n-1)*(2n-1)*C(n-2), C(0) = 1, C(n) = 0 for
 *            n < 0; a(n) = numerator(C(n)). Computed as C(n) = N(n)/Q(n) with
 *            Q(n) = Prod_{k<n} L(k), L(k) = 8(k+1)(2k+1)(2k+3), so that
 *            N(k+1) = A(k)*N(k) - B(k)*L(k-1)*N(k-1) - D(k)*L(k...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   g3282_addfac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/g3282_addfac.h"

typedef struct { uint32_t *d; size_t len, cap; } g3282_bn;

static void g3282_res(g3282_bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void g3282_trim(g3282_bn *x) {
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}

static void g3282_mul(g3282_bn *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) { g3282_res(x, x->len + 1); x->d[x->len++] = (uint32_t)c; }
}

static void g3282_copy(g3282_bn *dst, const g3282_bn *src) {
    g3282_res(dst, src->len);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void g3282_add(g3282_bn *x, const g3282_bn *y) {
    size_t n = x->len > y->len ? x->len : y->len, i;
    uint64_t c = 0;
    g3282_res(x, n + 1);
    for (i = x->len; i < n; i++) x->d[i] = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)x->d[i] + (i < y->len ? y->d[i] : 0) + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    x->len = n;
    if (c) x->d[x->len++] = (uint32_t)c;
}

/* x -= y, requires x >= y */
static void g3282_sub(g3282_bn *x, const g3282_bn *y) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < x->len; i++) {
        int64_t t = (int64_t)x->d[i] - (i < y->len ? (int64_t)y->d[i] : 0) - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        x->d[i] = (uint32_t)t;
    }
    g3282_trim(x);
}

static uint32_t g3282_mod(const g3282_bn *x, uint32_t m) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i > 0) { i--; r = ((r << 32) | x->d[i]) % m; }
    return (uint32_t)r;
}

static void g3282_div(g3282_bn *x, uint32_t m) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    g3282_trim(x);
}

static void g3282_shr(g3282_bn *x, uint64_t s) {
    size_t w = (size_t)(s / 32), i;
    unsigned b = (unsigned)(s % 32);
    if (w >= x->len) { x->len = 0; return; }
    for (i = 0; i + w < x->len; i++) {
        uint32_t lo = x->d[i + w] >> b;
        uint32_t hi = (b && i + w + 1 < x->len) ? (x->d[i + w + 1] << (32 - b)) : 0;
        x->d[i] = lo | hi;
    }
    x->len -= w;
    g3282_trim(x);
}

static void g3282_mulL(g3282_bn *x, uint32_t k) {
    g3282_mul(x, 8);
    g3282_mul(x, k + 1);
    g3282_mul(x, 2 * k + 1);
    g3282_mul(x, 2 * k + 3);
}

seq_t a(idx_t n) {
    g3282_bn B[5];
    g3282_bn *x0, *x1, *x2, *t, *s;
    uint32_t lim, i, k, nn;
    uint32_t *spf;
    long *ex;
    seq_t res = 0;
    size_t j;
    if (n == 0) return 1;
    nn = (uint32_t)n;
    for (i = 0; i < 5; i++) { B[i].d = NULL; B[i].len = 0; B[i].cap = 0; }
    x0 = &B[0]; x1 = &B[1]; x2 = &B[2]; t = &B[3]; s = &B[4];
    g3282_res(x0, 1); x0->d[0] = 1; x0->len = 1;

    for (k = 0; k < nn; k++) {
        uint64_t q5 = 5ULL * k * k + 5ULL * k + 2;
        g3282_copy(t, x0);
        g3282_mul(t, 6);
        g3282_mul(t, 2 * k + 1);
        g3282_mul(t, (uint32_t)q5);
        if (k >= 1) {
            g3282_copy(s, x1);
            g3282_mul(s, 24);
            g3282_mul(s, k); g3282_mul(s, k); g3282_mul(s, k);
            if (k >= 2) {
                g3282_mul(x2, 2);
                g3282_mul(x2, k);
                g3282_mul(x2, k - 1);
                g3282_mul(x2, 2 * k - 1);
                g3282_add(s, x2);
            }
            g3282_sub(t, s);
        }
        if (k + 1 < nn) {
            g3282_bn *tmp;
            g3282_mulL(x1, k);
            g3282_mulL(x0, k);
            tmp = x2; x2 = x1; x1 = x0; x0 = t; t = tmp;
        } else {
            g3282_bn *tmp = x0; x0 = t; t = tmp;
        }
    }

    /* exponents of Q(n) = Prod_{k<n} 8(k+1)(2k+1)(2k+3) */
    lim = 2 * nn + 2;
    spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    ex = (long *)calloc((size_t)lim + 1, sizeof(long));
    for (i = 2; i <= lim; i++)
        if (spf[i] == 0)
            for (j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = i;
    for (k = 0; k < nn; k++) {
        ex[2] += 3;
        g3282_addfac(ex, spf, k + 1);
        g3282_addfac(ex, spf, 2 * k + 1);
        g3282_addfac(ex, spf, 2 * k + 3);
    }

    /* remove powers of 2 */
    {
        uint64_t tz = 0;
        size_t w = 0;
        while (w < x0->len && x0->d[w] == 0) { tz += 32; w++; }
        if (w < x0->len) {
            uint32_t v = x0->d[w];
            while ((v & 1) == 0) { v >>= 1; tz++; }
        }
        if ((uint64_t)ex[2] < tz) tz = (uint64_t)ex[2];
        g3282_shr(x0, tz);
    }
    /* remove odd primes */
    for (i = 3; i <= lim; i++) {
        long e;
        if (spf[i] != i) continue;
        e = ex[i];
        while (e > 0) {
            uint64_t q = 1;
            long kk = 0;
            uint32_t r;
            while (kk < e && q * i <= 0xFFFFFFFFULL) { q *= i; kk++; }
            r = g3282_mod(x0, (uint32_t)q);
            if (r == 0) {
                g3282_div(x0, (uint32_t)q);
                e -= kk;
            } else {
                uint64_t pv = 1;
                while (r % i == 0) { r /= i; pv *= i; }
                if (pv > 1) g3282_div(x0, (uint32_t)pv);
                break;
            }
        }
    }

    j = x0->len;
    while (j > 0) {
        int sh;
        j--;
        for (sh = 24; sh >= 0; sh -= 8) {
            uint32_t byte = (x0->d[j] >> sh) & 0xFFu;
            res = (seq_t)(res * 256 + (seq_t)byte);
        }
    }

    free(spf);
    free(ex);
    for (i = 0; i < 5; i++) free(B[i].d);
    return res;
}
