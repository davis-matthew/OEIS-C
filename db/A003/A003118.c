/*
 * A003118  Continued fraction for fifth root of 4.
 * https://oeis.org/A003118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = n-th partial quotient of the continued fraction of
 *            4^(1/5), computed exactly with Lagrange's method on P(x) = x^5 -
 *            4: a = floor(real root of P), then P(y) <- y^5 * P(a + 1/y),
 *            using big-integer coefficients and no floating point
 * Range:     u8 n <= 265, u16 n <= 7250, u32 n <= 8376, u64 n <= 8376,
 *            u128 n <= 8376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *cf_C[6];
static uint32_t *cf_T;
static size_t cf_L = 0;
static uint64_t *cf_terms = NULL;
static size_t cf_n = 0, cf_cap = 0;

static int cf_neg(const uint32_t *x) { return (int)(x[cf_L - 1] >> 31); }

static size_t cf_used(const uint32_t *x) {
    uint32_t ext = cf_neg(x) ? 0xFFFFFFFFu : 0u;
    size_t i = cf_L;
    while (i > 0 && x[i - 1] == ext) i--;
    return i + 1;
}

static void cf_grow(size_t nl) {
    int i;
    size_t j;
    for (i = 0; i < 6; i++) {
        uint32_t ext = cf_neg(cf_C[i]) ? 0xFFFFFFFFu : 0u;
        uint32_t *p = (uint32_t *)realloc(cf_C[i], nl * sizeof(uint32_t));
        for (j = cf_L; j < nl; j++) p[j] = ext;
        cf_C[i] = p;
    }
    cf_T = (uint32_t *)realloc(cf_T, nl * sizeof(uint32_t));
    cf_L = nl;
}

static void cf_init(void) {
    int i;
    size_t j;
    cf_L = 4;
    for (i = 0; i < 6; i++) cf_C[i] = (uint32_t *)calloc(cf_L, sizeof(uint32_t));
    cf_T = (uint32_t *)calloc(cf_L, sizeof(uint32_t));
    cf_C[5][0] = 1;
    for (j = 0; j < cf_L; j++) cf_C[0][j] = 0xFFFFFFFFu;
    cf_C[0][0] = 0xFFFFFFFCu; /* -4 */
}

/* is the unique real root of P greater than k ? */
static int cf_above(uint32_t k) {
    int j;
    size_t i;
    memcpy(cf_T, cf_C[5], cf_L * sizeof(uint32_t));
    for (j = 4; j >= 0; j--) {
        uint64_t carry = 0;
        for (i = 0; i < cf_L; i++) {
            uint64_t t = (uint64_t)cf_T[i] * k + cf_C[j][i] + carry;
            cf_T[i] = (uint32_t)t;
            carry = t >> 32;
        }
    }
    return cf_neg(cf_T) != cf_neg(cf_C[5]);
}

static void cf_addmul(uint32_t *dst, const uint32_t *src, uint32_t k) {
    size_t i;
    uint64_t carry = 0;
    for (i = 0; i < cf_L; i++) {
        uint64_t t = (uint64_t)src[i] * k + dst[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void cf_step(void) {
    size_t mx = 0, u;
    int i, j;
    uint32_t lo, hi, a;
    uint32_t *tmp;
    for (i = 0; i < 6; i++) {
        u = cf_used(cf_C[i]);
        if (u > mx) mx = u;
    }
    if (mx + 8 > cf_L) cf_grow(2 * (mx + 8));

    lo = 1;
    hi = 2;
    while (hi < 0x80000000u && cf_above(hi)) {
        lo = hi;
        hi *= 2;
    }
    while (hi - lo > 1) {
        uint32_t mid = lo + (hi - lo) / 2;
        if (cf_above(mid)) lo = mid; else hi = mid;
    }
    a = lo;

    if (cf_n == cf_cap) {
        cf_cap = cf_cap ? cf_cap * 2 : 256;
        cf_terms = (uint64_t *)realloc(cf_terms, cf_cap * sizeof(uint64_t));
    }
    cf_terms[cf_n++] = a;

    /* Taylor shift P(x) -> P(x + a) */
    for (i = 0; i < 5; i++)
        for (j = 4; j >= i; j--)
            cf_addmul(cf_C[j], cf_C[j + 1], a);
    /* reverse: y^5 * P(a + 1/y) */
    tmp = cf_C[0]; cf_C[0] = cf_C[5]; cf_C[5] = tmp;
    tmp = cf_C[1]; cf_C[1] = cf_C[4]; cf_C[4] = tmp;
    tmp = cf_C[2]; cf_C[2] = cf_C[3]; cf_C[3] = tmp;
}

seq_t a(idx_t n) {
    if (cf_L == 0) cf_init();
    while ((idx_t)cf_n <= n) cf_step();
    return (seq_t)cf_terms[n];
}
