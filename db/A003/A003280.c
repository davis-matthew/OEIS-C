/*
 * A003280  Numerators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of B0(n), where 16*n*(n+1)*(2n+1)*B0(n+1) =
 *            n*(60n^2+9)*B0(n) - 3*(2n-1)^3*B0(n-1) -
 *            (n-1)*(2n-1)*(2n-3)*B0(n-2), B0(0) = 1, B0(1) = 9/32. Exact
 *            rational arithmetic: numerator as a big integer, reduced
 *            denominator kept as prime exponent vector
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t len; size_t cap; } a3280_big;

static void a3280_reserve(a3280_big *x, size_t c) {
    if (x->cap < c) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a3280_trim(a3280_big *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

static void a3280_set_u(a3280_big *x, uint32_t v) {
    a3280_reserve(x, 1);
    x->d[0] = v;
    x->len = v ? 1 : 0;
}

static void a3280_copy(a3280_big *dst, const a3280_big *src) {
    a3280_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void a3280_mul(a3280_big *x, uint64_t m) {
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    unsigned __int128 c = 0;
    for (size_t i = 0; i < x->len; i++) {
        c += (unsigned __int128)x->d[i] * m;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        a3280_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
        c >>= 32;
    }
}

static void a3280_add(a3280_big *a, const a3280_big *b) {
    size_t m = a->len > b->len ? a->len : b->len;
    a3280_reserve(a, m + 1);
    for (size_t i = a->len; i < m + 1; i++) a->d[i] = 0;
    uint64_t c = 0;
    for (size_t i = 0; i < m; i++) {
        c += (uint64_t)a->d[i] + (i < b->len ? b->d[i] : 0);
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    a->d[m] = (uint32_t)c;
    a->len = m + 1;
    a3280_trim(a);
}

/* a -= b, requires a >= b */
static void a3280_sub(a3280_big *a, const a3280_big *b) {
    int64_t br = 0;
    for (size_t i = 0; i < a->len; i++) {
        int64_t v = (int64_t)a->d[i] - (i < b->len ? (int64_t)b->d[i] : 0) + br;
        if (v < 0) { v += (int64_t)1 << 32; br = -1; } else br = 0;
        a->d[i] = (uint32_t)v;
    }
    a3280_trim(a);
}

static void a3280_div(a3280_big *x, uint32_t dv) {
    uint64_t r = 0;
    for (size_t i = x->len; i-- > 0;) {
        r = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(r / dv);
        r %= dv;
    }
    a3280_trim(x);
}

static uint32_t a3280_mod(const a3280_big *x, uint32_t p) {
    uint64_t r = 0;
    for (size_t i = x->len; i-- > 0;) r = ((r << 32) | x->d[i]) % p;
    return (uint32_t)r;
}

static uint64_t a3280_ctz(const a3280_big *x) {
    uint64_t t = 0;
    size_t i = 0;
    while (i < x->len && x->d[i] == 0) { t += 32; i++; }
    if (i < x->len) {
        uint32_t v = x->d[i];
        while (!(v & 1u)) { v >>= 1; t++; }
    }
    return t;
}

static void a3280_shr(a3280_big *x, uint64_t s) {
    if (s == 0 || x->len == 0) return;
    size_t words = (size_t)(s / 32);
    unsigned bits = (unsigned)(s % 32);
    if (words >= x->len) { x->len = 0; return; }
    size_t nl = x->len - words;
    for (size_t i = 0; i < nl; i++) {
        uint64_t lo = x->d[i + words];
        uint64_t hi = (i + words + 1 < x->len) ? x->d[i + words + 1] : 0;
        x->d[i] = (uint32_t)(((hi << 32) | lo) >> bits);
    }
    x->len = nl;
    a3280_trim(x);
}

static uint32_t *a3280_spf = NULL;
static int *a3280_pidx = NULL;
static uint32_t *a3280_primes = NULL;
static size_t a3280_S = 0, a3280_np = 0;

static a3280_big a3280_N[3], a3280_T1, a3280_T2, a3280_T3;
static int *a3280_E[3] = {NULL, NULL, NULL};
static int *a3280_L = NULL;
static idx_t a3280_k = 0;

static void a3280_sieve(size_t lim) {
    a3280_spf = (uint32_t *)realloc(a3280_spf, (lim + 1) * sizeof(uint32_t));
    a3280_pidx = (int *)realloc(a3280_pidx, (lim + 1) * sizeof(int));
    a3280_primes = (uint32_t *)realloc(a3280_primes, (lim + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= lim; i++) { a3280_spf[i] = 0; a3280_pidx[i] = -1; }
    a3280_np = 0;
    for (size_t i = 2; i <= lim; i++) {
        if (a3280_spf[i] == 0) {
            a3280_pidx[i] = (int)a3280_np;
            a3280_primes[a3280_np++] = (uint32_t)i;
            for (size_t j = i; j <= lim; j += i)
                if (a3280_spf[j] == 0) a3280_spf[j] = (uint32_t)i;
        }
    }
    a3280_S = lim;
    for (int i = 0; i < 3; i++)
        a3280_E[i] = (int *)realloc(a3280_E[i], (a3280_np + 1) * sizeof(int));
    a3280_L = (int *)realloc(a3280_L, (a3280_np + 1) * sizeof(int));
}

static void a3280_addf(int *e, uint64_t x) {
    while (x > 1) {
        uint32_t p = a3280_spf[x];
        e[a3280_pidx[p]]++;
        x /= p;
    }
}

static void a3280_init(idx_t n) {
    size_t need = (size_t)(2 * n + 8);
    if (a3280_S < need) {
        size_t ns = a3280_S * 2;
        if (ns < need) ns = need;
        if (ns < 64) ns = 64;
        a3280_sieve(ns);
    }
    for (int i = 0; i < 3; i++)
        for (size_t j = 0; j < a3280_np; j++) a3280_E[i][j] = 0;
    a3280_set_u(&a3280_N[0], 0);
    a3280_set_u(&a3280_N[1], 1);
    a3280_set_u(&a3280_N[2], 9);
    a3280_E[2][0] = 5; /* 9/32 */
    a3280_k = 1;
}

static void a3280_scale(a3280_big *t, const a3280_big *src, uint64_t c, const int *e) {
    a3280_copy(t, src);
    a3280_mul(t, c);
    if (t->len == 0) return;
    uint64_t acc = 1;
    for (size_t j = 0; j < a3280_np; j++) {
        int diff = a3280_L[j] - e[j];
        uint64_t p = a3280_primes[j];
        for (int q = 0; q < diff; q++) {
            if (acc > (UINT64_C(1) << 40)) { a3280_mul(t, acc); acc = 1; }
            acc *= p;
        }
    }
    if (acc > 1) a3280_mul(t, acc);
}

static void a3280_step(void) {
    uint64_t k = (uint64_t)a3280_k;
    for (size_t j = 0; j < a3280_np; j++) {
        int m = a3280_E[0][j];
        if (a3280_E[1][j] > m) m = a3280_E[1][j];
        if (a3280_E[2][j] > m) m = a3280_E[2][j];
        a3280_L[j] = m;
    }
    uint64_t c1 = k * (60 * k * k + 9);
    uint64_t c2 = 3 * (2 * k - 1) * (2 * k - 1) * (2 * k - 1);
    uint64_t c3 = (k >= 2) ? (k - 1) * (2 * k - 1) * (2 * k - 3) : 0;

    a3280_scale(&a3280_T1, &a3280_N[2], c1, a3280_E[2]);
    a3280_scale(&a3280_T2, &a3280_N[1], c2, a3280_E[1]);
    a3280_scale(&a3280_T3, &a3280_N[0], c3, a3280_E[0]);
    a3280_add(&a3280_T2, &a3280_T3);
    a3280_sub(&a3280_T1, &a3280_T2);

    /* denominator *= 16*k*(k+1)*(2k+1) */
    a3280_L[0] += 4;
    a3280_addf(a3280_L, k);
    a3280_addf(a3280_L, k + 1);
    a3280_addf(a3280_L, 2 * k + 1);

    /* reduce */
    if (a3280_L[0] > 0) {
        uint64_t t = a3280_ctz(&a3280_T1);
        if (t > (uint64_t)a3280_L[0]) t = (uint64_t)a3280_L[0];
        a3280_shr(&a3280_T1, t);
        a3280_L[0] -= (int)t;
    }
    for (size_t j = 1; j < a3280_np; j++) {
        uint32_t p = a3280_primes[j];
        while (a3280_L[j] > 0 && a3280_mod(&a3280_T1, p) == 0) {
            a3280_div(&a3280_T1, p);
            a3280_L[j]--;
        }
    }

    a3280_big tmp = a3280_N[0];
    a3280_N[0] = a3280_N[1];
    a3280_N[1] = a3280_N[2];
    a3280_N[2] = a3280_T1;
    a3280_T1 = tmp;
    int *te = a3280_E[0];
    a3280_E[0] = a3280_E[1];
    a3280_E[1] = a3280_E[2];
    a3280_E[2] = a3280_L;
    a3280_L = te;
    a3280_k++;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (a3280_k == 0 || n < a3280_k || (size_t)(2 * n + 8) > a3280_S) a3280_init(n);
    while (a3280_k < n) a3280_step();
    const a3280_big *x = &a3280_N[2];
    seq_t r = 0;
    for (size_t i = x->len; i-- > 0;) {
        uint32_t limb = x->d[i];
        r = (seq_t)(r * 65536);
        r = (seq_t)(r + (seq_t)(limb >> 16));
        r = (seq_t)(r * 65536);
        r = (seq_t)(r + (seq_t)(limb & 0xFFFFu));
    }
    return r;
}
