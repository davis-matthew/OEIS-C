/*
 * A003284  Numerators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(f(n)), where f(0) = 1, f(1) = 1/18 and
 *            36*n*(n+1)*(2n+1)*f(n+1) = 4*n*(20*n^2+1)*f(n) -
 *            (2n-1)^3*f(n-1). Writing f(n) = N(n)/D(n) with D(n) = 36^n * n!
 *            * (2n-1)!!: N(0) = 1, N(1) = 2, N(n+1) = 4*(20n^2+1)*N(n) -
 *            36*(2n-1)^4*N(n-1); a(n) = N(n)/gcd(N(n), D(n)), the gcd found
 *            prime...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   g3284_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/g3284_leg.h"

typedef struct { uint32_t *d; size_t len, cap; } g3284_bn;

static void g3284_res(g3284_bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = x->cap * 2;
        if (nc < c) nc = c;
        if (nc < 4) nc = 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void g3284_set(g3284_bn *x, uint32_t v) {
    g3284_res(x, 1);
    x->d[0] = v;
    x->len = 1;
}

static void g3284_copy(g3284_bn *dst, const g3284_bn *src) {
    g3284_res(dst, src->len);
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void g3284_trim(g3284_bn *x) {
    while (x->len > 1 && x->d[x->len - 1] == 0) x->len--;
}

static void g3284_mul(g3284_bn *x, uint64_t m) {
    unsigned __int128 c = 0;
    size_t i;
    for (i = 0; i < x->len; i++) {
        c += (unsigned __int128)x->d[i] * m;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        g3284_res(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
        c >>= 32;
    }
    g3284_trim(x);
}

/* x -= y, assumes x >= y */
static void g3284_sub(g3284_bn *x, const g3284_bn *y) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < x->len; i++) {
        uint64_t yi = i < y->len ? y->d[i] : 0;
        uint64_t t = (uint64_t)x->d[i] - yi - br;
        x->d[i] = (uint32_t)t;
        br = (t >> 32) ? 1 : 0;
    }
    g3284_trim(x);
}

static uint64_t g3284_mod(const g3284_bn *x, uint64_t q) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) r = ((r << 32) | x->d[i]) % q;
    return r;
}

static void g3284_div(g3284_bn *x, uint64_t q) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / q);
        r = cur % q;
    }
    g3284_trim(x);
}

static g3284_bn g3284_cur = {NULL, 0, 0};
static g3284_bn g3284_prv = {NULL, 0, 0};
static g3284_bn g3284_tmp = {NULL, 0, 0};
static g3284_bn g3284_s = {NULL, 0, 0};
static g3284_bn g3284_r = {NULL, 0, 0};
static idx_t g3284_k = 0;
static int g3284_init = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (!g3284_init || n < g3284_k) {
        g3284_set(&g3284_prv, 1);
        g3284_set(&g3284_cur, 2);
        g3284_k = 1;
        g3284_init = 1;
    }
    while (g3284_k < n) {
        uint64_t k = (uint64_t)g3284_k;
        uint64_t t = 2 * k - 1;
        g3284_bn sw;
        g3284_copy(&g3284_tmp, &g3284_prv);
        g3284_mul(&g3284_tmp, 36 * t * t);
        g3284_mul(&g3284_tmp, t * t);
        g3284_copy(&g3284_s, &g3284_cur);
        g3284_mul(&g3284_s, 4 * (20 * k * k + 1));
        g3284_sub(&g3284_s, &g3284_tmp);
        /* prv <- cur, cur <- s, s <- old prv buffer */
        sw = g3284_prv;
        g3284_prv = g3284_cur;
        g3284_cur = g3284_s;
        g3284_s = sw;
        g3284_k++;
    }

    g3284_copy(&g3284_r, &g3284_cur);
    {
        uint64_t nn = (uint64_t)n;
        uint64_t lim = 2 * nn;
        uint64_t p, i;
        unsigned char *comp;
        if (lim < 3) lim = 3;
        comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        for (p = 2; p <= lim; p++) {
            uint64_t e;
            if (comp[p]) continue;
            for (i = p * p; i <= lim; i += p) comp[i] = 1;
            if (p == 2) e = 2 * nn + g3284_leg(nn, 2);
            else e = (p == 3 ? 2 * nn : 0) + g3284_leg(2 * nn, p);
            while (e > 0) {
                uint64_t q = 1, kk = 0;
                while (kk < e && q * p <= 0xFFFFFFFFULL) { q *= p; kk++; }
                if (g3284_mod(&g3284_r, q) == 0) {
                    g3284_div(&g3284_r, q);
                    e -= kk;
                } else {
                    while (e > 0 && g3284_mod(&g3284_r, p) == 0) {
                        g3284_div(&g3284_r, p);
                        e--;
                    }
                    break;
                }
            }
        }
        free(comp);
    }

    {
        seq_t x = 0;
        size_t i = g3284_r.len;
        while (i--) {
            x = x * 65536u;
            x = x * 65536u;
            x += g3284_r.d[i];
        }
        return x;
    }
}
