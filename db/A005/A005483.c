/*
 * A005483  Continued fraction for cube root of 7.
 * https://oeis.org/A005483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction of the real root of x^3 - 7, computed exactly
 *            by Lagrange's method: a(n) = floor(r_n) where r_0 = 7^(1/3) and
 *            r_{n+1} = 1/(r_n - a(n)); each r_n is tracked as the unique real
 *            root of an integer cubic, transformed by x -> a(n) + 1/y at each
 *            step
 * Range:     u8 n <= 27, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   mag_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mag_cmp.h"

typedef struct { int neg; size_t len, cap; uint32_t *d; } cf_bn;

static cf_bn P[4], T[4];
static uint64_t *terms = NULL;
static size_t nterms = 0, capterms = 0;
static int inited = 0;
static uint32_t *scratch = NULL;
static size_t scratch_cap = 0;

static void bn_reserve(cf_bn *a, size_t n) {
    if (a->cap < n) {
        size_t c = a->cap ? a->cap : 8;
        while (c < n) c *= 2;
        a->d = (uint32_t *)realloc(a->d, c * sizeof(uint32_t));
        a->cap = c;
    }
}

static void bn_trim(cf_bn *a) {
    while (a->len && a->d[a->len - 1] == 0) a->len--;
    if (!a->len) a->neg = 0;
}

static void bn_copy(cf_bn *dst, const cf_bn *src) {
    bn_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
    dst->neg = src->neg;
}

/* c += t * b */
static void bn_addmul(cf_bn *c, const cf_bn *b, uint32_t t) {
    size_t lm, i;
    uint64_t carry = 0;
    int mneg = b->neg;
    if (b->len == 0 || t == 0) return;
    if (scratch_cap < b->len + 1) {
        scratch_cap = 2 * (b->len + 1);
        scratch = (uint32_t *)realloc(scratch, scratch_cap * sizeof(uint32_t));
    }
    for (i = 0; i < b->len; i++) {
        uint64_t v = (uint64_t)b->d[i] * t + carry;
        scratch[i] = (uint32_t)v;
        carry = v >> 32;
    }
    lm = b->len;
    if (carry) scratch[lm++] = (uint32_t)carry;

    if (c->len == 0 || c->neg == mneg) {
        size_t n = (c->len > lm ? c->len : lm);
        uint64_t cr = 0;
        bn_reserve(c, n + 1);
        for (i = c->len; i < n + 1; i++) c->d[i] = 0;
        for (i = 0; i < n; i++) {
            uint64_t v = (uint64_t)c->d[i] + (i < lm ? scratch[i] : 0) + cr;
            c->d[i] = (uint32_t)v;
            cr = v >> 32;
        }
        c->d[n] = (uint32_t)cr;
        c->len = n + 1;
        c->neg = mneg;
    } else {
        int cmp = mag_cmp(c->d, c->len, scratch, lm);
        int64_t borrow = 0;
        if (cmp >= 0) {
            for (i = 0; i < c->len; i++) {
                int64_t v = (int64_t)c->d[i] - (i < lm ? scratch[i] : 0) - borrow;
                if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
                c->d[i] = (uint32_t)v;
            }
        } else {
            size_t oldlen = c->len;
            bn_reserve(c, lm);
            for (i = 0; i < lm; i++) {
                int64_t ci = (i < oldlen) ? c->d[i] : 0;
                int64_t v = (int64_t)scratch[i] - ci - borrow;
                if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
                c->d[i] = (uint32_t)v;
            }
            c->len = lm;
            c->neg = mneg;
        }
    }
    bn_trim(c);
}

/* p(x) -> p(x + t), coefficients p[0]*x^3 + p[1]*x^2 + p[2]*x + p[3] */
static void poly_shift(cf_bn *p, uint32_t t) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 1; j <= 3 - i; j++)
            bn_addmul(&p[j], &p[j - 1], t);
}

static void swap_bn(cf_bn *x, cf_bn *y) {
    cf_bn tmp = *x;
    *x = *y;
    *y = tmp;
}

static int try_shift(uint32_t step, int s) {
    int i;
    for (i = 0; i < 4; i++) bn_copy(&T[i], &P[i]);
    poly_shift(T, step);
    if (T[3].len && T[3].neg == s) {
        for (i = 0; i < 4; i++) swap_bn(&P[i], &T[i]);
        return 1;
    }
    return 0;
}

static void next_term(void) {
    int s = P[3].neg;
    uint64_t q = 0;
    uint32_t step = 1;
    while (try_shift(step, s)) {
        q += step;
        if (step < 0x80000000u) step *= 2;
    }
    step /= 2;
    while (step) {
        if (try_shift(step, s)) q += step;
        step /= 2;
    }
    if (nterms == capterms) {
        capterms = capterms ? 2 * capterms : 64;
        terms = (uint64_t *)realloc(terms, capterms * sizeof(uint64_t));
    }
    terms[nterms++] = q;
    /* y^3 * p(q + 1/y): reverse coefficients */
    swap_bn(&P[0], &P[3]);
    swap_bn(&P[1], &P[2]);
}

seq_t a(idx_t n) {
    if (!inited) {
        bn_reserve(&P[0], 1); P[0].d[0] = 1; P[0].len = 1; P[0].neg = 0;
        P[1].len = 0; P[1].neg = 0;
        P[2].len = 0; P[2].neg = 0;
        bn_reserve(&P[3], 1); P[3].d[0] = 7; P[3].len = 1; P[3].neg = 1;
        inited = 1;
    }
    while ((idx_t)nterms <= n) next_term();
    return (seq_t)terms[n];
}
