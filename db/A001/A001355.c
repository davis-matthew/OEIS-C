/*
 * A001355  Mix digits of Pi and e.
 * https://oeis.org/A001355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, dumb, easy
 * Formula:   a(2k-1) = k-th digit of Pi, a(2k) = k-th digit of e (k >= 1).
 *            The digits are computed with exact fixed-point arithmetic in
 *            base 10^4: Pi = 16*arctan(1/5) - 4*arctan(1/239) (Machin), e =
 *            Sum_{k>=0} 1/k!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1355_digit, a1355_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1355_digit.h"
#include "../utils/a1355_iszero.h"

#define A1355_B 10000u

static void a1355_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t c = r * A1355_B + x[i];
        x[i] = (uint32_t)(c / d);
        r = c % d;
    }
}

static void a1355_add(uint32_t *s, const uint32_t *t, size_t L) {
    uint32_t c = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        uint32_t v = s[i] + t[i] + c;
        if (v >= A1355_B) { v -= A1355_B; c = 1; } else c = 0;
        s[i] = v;
    }
}

static void a1355_sub(uint32_t *s, const uint32_t *t, size_t L) {
    int64_t b = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        int64_t v = (int64_t)s[i] - (int64_t)t[i] - b;
        if (v < 0) { v += A1355_B; b = 1; } else b = 0;
        s[i] = (uint32_t)v;
    }
}

/* sum += sgn * mult * arctan(1/x) */
static void a1355_atan(uint32_t *sum, uint32_t *t, uint32_t *u, size_t L,
                       uint32_t x, uint32_t mult, int sgn) {
    uint64_t k = 0;
    memset(t, 0, L * sizeof(uint32_t));
    t[0] = mult;
    a1355_divs(t, L, x);
    while (!a1355_iszero(t, L)) {
        memcpy(u, t, L * sizeof(uint32_t));
        a1355_divs(u, L, (uint32_t)(2 * k + 1));
        if (((k & 1) == 0) == (sgn > 0)) a1355_add(sum, u, L);
        else a1355_sub(sum, u, L);
        a1355_divs(t, L, x * x);
        k++;
    }
}

static uint32_t *a1355_pi = NULL, *a1355_e = NULL;
static idx_t a1355_D = 0;

static void a1355_build(idx_t D) {
    size_t L = (size_t)(1 + (D + 3) / 4 + 4);
    uint32_t *t = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *u = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *e = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t k;
    a1355_atan(p, t, u, L, 5, 16, 1);
    a1355_atan(p, t, u, L, 239, 4, -1);
    memset(t, 0, L * sizeof(uint32_t));
    t[0] = 1;
    a1355_add(e, t, L);
    for (k = 1;; k++) {
        a1355_divs(t, L, k);
        if (a1355_iszero(t, L)) break;
        a1355_add(e, t, L);
    }
    free(t);
    free(u);
    free(a1355_pi);
    free(a1355_e);
    a1355_pi = p;
    a1355_e = e;
    a1355_D = D;
}

seq_t a(idx_t n) {
    idx_t d;
    const uint32_t *src;
    if (n < 1) return 0;
    d = (n % 2 == 1) ? (n - 1) / 2 : n / 2 - 1;
    if (d + 1 > a1355_D) {
        idx_t D = 2 * d + 100;
        if (D < 2000) D = 2000;
        a1355_build(D);
    }
    src = (n % 2 == 1) ? a1355_pi : a1355_e;
    return (seq_t)a1355_digit(src, d);
}
