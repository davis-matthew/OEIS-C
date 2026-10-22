/*
 * A002363  Denominators of continued fraction convergents to fifth root of 5.
 * https://oeis.org/A002363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) is
 *            the n-th continued fraction partial quotient of 5^(1/5); the
 *            c(n) are computed exactly by Lagrange's method on x^5 - 5
 *            (Taylor shifts p(x) -> p(x+c), then x -> 1/x), using
 *            integer-only multiprecision polynomial coefficients
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 44, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *cf_P[6];
static uint32_t *cf_T = NULL, *cf_U = NULL;
static size_t cf_L = 0;
static uint64_t *cf_A = NULL;
static size_t cf_na = 0, cf_capa = 0;

static void cf_grow(size_t nl) {
    int k;
    size_t i;
    for (k = 0; k < 6; k++) {
        uint32_t fill = (cf_L && (cf_P[k][cf_L - 1] >> 31)) ? 0xFFFFFFFFu : 0u;
        cf_P[k] = (uint32_t *)realloc(cf_P[k], nl * sizeof(uint32_t));
        for (i = cf_L; i < nl; i++) cf_P[k][i] = fill;
    }
    cf_T = (uint32_t *)realloc(cf_T, nl * sizeof(uint32_t));
    cf_U = (uint32_t *)realloc(cf_U, nl * sizeof(uint32_t));
    cf_L = nl;
}

static size_t cf_used(void) {
    size_t m = 1;
    int k;
    for (k = 0; k < 6; k++) {
        uint32_t f = (cf_P[k][cf_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        size_t i = cf_L;
        while (i > 0 && cf_P[k][i - 1] == f) i--;
        if (i + 1 > m) m = i + 1;
    }
    return m;
}

static void cf_ensure(size_t bits) {
    size_t need = cf_used() + bits / 32 + 2;
    if (need > cf_L) cf_grow(need + need / 8 + 4);
}

/* d += s << j  (two's complement, length cf_L) */
static void cf_add_shl(uint32_t *d, const uint32_t *s, unsigned j) {
    size_t w = j / 32, i;
    unsigned b = j % 32;
    uint64_t carry = 0;
    for (i = 0; i < cf_L; i++) {
        uint32_t v = 0;
        if (i >= w) {
            v = b ? (s[i - w] << b) : s[i - w];
            if (b && i > w) v |= s[i - w - 1] >> (32 - b);
        }
        {
            uint64_t t = (uint64_t)d[i] + v + carry;
            d[i] = (uint32_t)t;
            carry = t >> 32;
        }
    }
}

/* sign bit of p(2^j) */
static unsigned cf_eval_sign(unsigned j) {
    int k;
    memcpy(cf_T, cf_P[5], cf_L * sizeof(uint32_t));
    for (k = 4; k >= 0; k--) {
        uint32_t *tmp;
        memcpy(cf_U, cf_P[k], cf_L * sizeof(uint32_t));
        cf_add_shl(cf_U, cf_T, j);
        tmp = cf_T; cf_T = cf_U; cf_U = tmp;
    }
    return cf_T[cf_L - 1] >> 31;
}

/* p(x) := p(x + 2^j) */
static void cf_shift(unsigned j) {
    int i, k;
    for (i = 0; i < 5; i++)
        for (k = 4; k >= i; k--)
            cf_add_shl(cf_P[k], cf_P[k + 1], j);
}

static void cf_init(void) {
    size_t i;
    cf_grow(4);
    for (i = 0; i < cf_L; i++) cf_P[0][i] = 0xFFFFFFFFu;
    cf_P[0][0] = 0xFFFFFFFBu; /* -5 */
    cf_P[5][0] = 1u;          /* x^5 */
}

static void cf_step(void) {
    unsigned s0, j = 0;
    int up = 1, k;
    uint64_t q = 0;
    cf_ensure(40);
    s0 = cf_P[0][cf_L - 1] >> 31;
    for (;;) {
        int accept;
        cf_ensure(5 * (size_t)j + 40);
        accept = (cf_eval_sign(j) == s0);
        if (accept) {
            cf_shift(j);
            q += (uint64_t)1 << j;
        }
        if (up && accept) { j++; continue; }
        up = 0;
        if (j == 0) break;
        j--;
    }
    for (k = 0; k < 3; k++) {
        uint32_t *tmp = cf_P[k];
        cf_P[k] = cf_P[5 - k];
        cf_P[5 - k] = tmp;
    }
    if (cf_na == cf_capa) {
        cf_capa = cf_capa ? 2 * cf_capa : 64;
        cf_A = (uint64_t *)realloc(cf_A, cf_capa * sizeof(uint64_t));
    }
    cf_A[cf_na++] = q;
}

seq_t a(idx_t n) {
    seq_t q0, q1;
    idx_t i;
    if (cf_L == 0) cf_init();
    while ((idx_t)cf_na <= n) cf_step();
    q0 = 0;
    q1 = 1;
    for (i = 1; i <= n; i++) {
        seq_t t = (seq_t)cf_A[i] * q1 + q0;
        q0 = q1;
        q1 = t;
    }
    return q1;
}
