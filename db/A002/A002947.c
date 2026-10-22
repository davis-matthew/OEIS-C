/*
 * A002947  Continued fraction for cube root of 4.
 * https://oeis.org/A002947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th partial quotient of the continued fraction of
 *            4^(1/3), found exactly by Lagrange's method: start from P(x) =
 *            x^3 - 4; repeatedly take a = floor(real root of P) (largest
 *            integer a with P(a) < 0), shift x -> x + a, then invert x -> 1/x
 *            (reverse and negate the coefficients). Coefficients...
 * Range:     u8 n <= 39, u16 n <= 9352, u32 n <= 9352, u64 n <= 9352,
 *            u128 n <= 9352, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *c47_P[4], *c47_Q[4];
static size_t c47_W = 0;
static uint64_t *c47_t = NULL;
static idx_t c47_n = 0, c47_cap = 0;

static int c47_isneg(const uint32_t *x) { return (int)(x[c47_W - 1] >> 31); }

/* x += s*y  (mod 2^(32W)) */
static void c47_addmul(uint32_t *x, const uint32_t *y, uint32_t s) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < c47_W; i++) {
        uint64_t t = (uint64_t)x[i] + (uint64_t)y[i] * s + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

/* Taylor shift p(x) -> p(x+s) by synthetic division */
static void c47_tshift(uint32_t **p, uint32_t s) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 2; j >= i; j--)
            c47_addmul(p[j], p[j + 1], s);
}

static void c47_ensure(void) {
    int k, ok = 1;
    size_t i, nw;
    uint32_t e[4];
    for (k = 0; k < 4; k++) {
        e[k] = c47_isneg(c47_P[k]) ? 0xFFFFFFFFu : 0u;
        for (i = c47_W - 4; i < c47_W; i++)
            if (c47_P[k][i] != e[k]) ok = 0;
    }
    if (ok) return;
    nw = c47_W + 8;
    for (k = 0; k < 4; k++) {
        c47_P[k] = (uint32_t *)realloc(c47_P[k], nw * sizeof(uint32_t));
        for (i = c47_W; i < nw; i++) c47_P[k][i] = e[k];
        c47_Q[k] = (uint32_t *)realloc(c47_Q[k], nw * sizeof(uint32_t));
    }
    c47_W = nw;
}

static void c47_swap(void) {
    int k;
    for (k = 0; k < 4; k++) {
        uint32_t *tmp = c47_P[k];
        c47_P[k] = c47_Q[k];
        c47_Q[k] = tmp;
    }
}

static uint64_t c47_next(void) {
    uint64_t a = 0;
    uint32_t s = 1;
    int grow = 1, k;
    size_t i;
    for (;;) {
        c47_ensure();
        for (k = 0; k < 4; k++) memcpy(c47_Q[k], c47_P[k], c47_W * sizeof(uint32_t));
        c47_tshift(c47_Q, s);
        if (c47_isneg(c47_Q[0])) {
            c47_swap();
            a += s;
            if (grow && s < (1u << 30)) s <<= 1;
        } else {
            if (s == 1) break;
            s >>= 1;
            grow = 0;
        }
    }
    /* now P(0) < 0 and root in (0,1): R(y) = -y^3 P(1/y) */
    for (k = 0; k < 4; k++) {
        uint64_t carry = 1;
        for (i = 0; i < c47_W; i++) {
            uint64_t t = (uint64_t)(uint32_t)~c47_P[3 - k][i] + carry;
            c47_Q[k][i] = (uint32_t)t;
            carry = t >> 32;
        }
    }
    c47_swap();
    return a;
}

seq_t a(idx_t n) {
    if (c47_W == 0) {
        int k;
        size_t i;
        c47_W = 8;
        for (k = 0; k < 4; k++) {
            c47_P[k] = (uint32_t *)calloc(c47_W, sizeof(uint32_t));
            c47_Q[k] = (uint32_t *)calloc(c47_W, sizeof(uint32_t));
        }
        c47_P[3][0] = 1;
        for (i = 0; i < c47_W; i++) c47_P[0][i] = 0xFFFFFFFFu;
        c47_P[0][0] = 0xFFFFFFFCu; /* -4 */
    }
    while (c47_n <= n) {
        if (c47_n >= c47_cap) {
            idx_t nc = c47_cap ? c47_cap * 2 : 256;
            c47_t = (uint64_t *)realloc(c47_t, (size_t)nc * sizeof(uint64_t));
            c47_cap = nc;
        }
        c47_t[c47_n] = c47_next();
        c47_n++;
    }
    return (seq_t)c47_t[n];
}
