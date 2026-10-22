/*
 * A002354  Numerators of convergents to cube root of 3.
 * https://oeis.org/A002354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = 1, where c(n) are
 *            the continued-fraction partial quotients of 3^(1/3), generated
 *            exactly by Lagrange's method on x^3 - 3 (shift x -> c + 1/y with
 *            big-integer coefficients)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint32_t *a2354_C[4];
static uint32_t *a2354_T;
static size_t a2354_L = 0;
static uint64_t *a2354_cf = NULL;
static size_t a2354_ncf = 0, a2354_cap = 0;

/* dst = x*m + y  (mod 2^(32L)), two's complement; aliasing allowed */
static void a2354_muladd(uint32_t *dst, const uint32_t *x, uint64_t m, const uint32_t *y) {
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < a2354_L; i++) {
        unsigned __int128 t = (unsigned __int128)x[i] * m + y[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a2354_grow(size_t newL) {
    int k;
    size_t i;
    for (k = 0; k < 4; k++) {
        uint32_t fill = (a2354_C[k][a2354_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        a2354_C[k] = (uint32_t *)realloc(a2354_C[k], newL * sizeof(uint32_t));
        for (i = a2354_L; i < newL; i++) a2354_C[k][i] = fill;
    }
    a2354_T = (uint32_t *)realloc(a2354_T, newL * sizeof(uint32_t));
    a2354_L = newL;
}

static void a2354_headroom(void) {
    int k;
    size_t i;
    for (k = 0; k < 4; k++) {
        uint32_t fill = (a2354_C[k][a2354_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        for (i = a2354_L - 9; i < a2354_L; i++) {
            if (a2354_C[k][i] != fill) { a2354_grow(a2354_L + 16); return; }
        }
    }
}

static double a2354_tod(const uint32_t *x, long *e) {
    uint32_t neg = x[a2354_L - 1] >> 31;
    uint32_t fill = neg ? 0xFFFFFFFFu : 0u;
    size_t t = a2354_L - 1, k, i;
    double d;
    while (t > 0 && x[t] == fill && (x[t - 1] >> 31) == neg) t--;
    k = t >= 2 ? t - 2 : 0;
    d = (double)(int32_t)x[t];
    for (i = t; i > k; i--) d = d * 4294967296.0 + (double)x[i - 1];
    *e = (long)(32 * k);
    return d;
}

static int a2354_evneg(uint64_t v) {
    int k;
    memcpy(a2354_T, a2354_C[0], a2354_L * sizeof(uint32_t));
    for (k = 1; k < 4; k++) a2354_muladd(a2354_T, a2354_T, v, a2354_C[k]);
    return (int)(a2354_T[a2354_L - 1] >> 31);
}

static void a2354_step(void) {
    long e0, e1;
    double d0, d1, est;
    uint64_t c;
    int sc, i, j;
    uint32_t *tp;

    a2354_headroom();
    d0 = a2354_tod(a2354_C[0], &e0);
    d1 = a2354_tod(a2354_C[1], &e1);
    est = -ldexp(d1 / d0, (int)(e1 - e0));
    if (!(est >= 1.0)) est = 1.0;
    if (est > 1e18) est = 1e18;
    c = (uint64_t)est;

    sc = (int)(a2354_C[0][a2354_L - 1] >> 31);
    while (c > 1 && a2354_evneg(c) == sc) c--;
    while (a2354_evneg(c + 1) != sc) c++;

    if (a2354_ncf == a2354_cap) {
        a2354_cap = a2354_cap ? 2 * a2354_cap : 256;
        a2354_cf = (uint64_t *)realloc(a2354_cf, a2354_cap * sizeof(uint64_t));
    }
    a2354_cf[a2354_ncf++] = c;

    /* Taylor shift: Q(x) -> Q(x + c) */
    for (i = 0; i < 3; i++)
        for (j = 1; j <= 3 - i; j++)
            a2354_muladd(a2354_C[j], a2354_C[j - 1], c, a2354_C[j]);
    /* reverse: y^3 * R(1/y) */
    tp = a2354_C[0]; a2354_C[0] = a2354_C[3]; a2354_C[3] = tp;
    tp = a2354_C[1]; a2354_C[1] = a2354_C[2]; a2354_C[2] = tp;
}

static void a2354_init(void) {
    int k;
    size_t i;
    a2354_L = 16;
    for (k = 0; k < 4; k++)
        a2354_C[k] = (uint32_t *)calloc(a2354_L, sizeof(uint32_t));
    a2354_T = (uint32_t *)calloc(a2354_L, sizeof(uint32_t));
    a2354_C[0][0] = 1;                       /* x^3 */
    for (i = 0; i < a2354_L; i++) a2354_C[3][i] = 0xFFFFFFFFu;
    a2354_C[3][0] = 0xFFFFFFFDu;             /* -3 */
}

seq_t a(idx_t n) {
    seq_t p0, p1, t;
    idx_t i;
    if (a2354_L == 0) a2354_init();
    while (a2354_ncf <= n) a2354_step();

    p0 = 1;
    p1 = (seq_t)a2354_cf[0];
    for (i = 1; i <= n; i++) {
        t = (seq_t)a2354_cf[i] * p1 + p0;
        p0 = p1;
        p1 = t;
    }
    return p1;
}
