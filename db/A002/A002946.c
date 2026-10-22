/*
 * A002946  Continued fraction for cube root of 3.
 * https://oeis.org/A002946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction of 3^(1/3) by Lagrange's method: start with
 *            P(x) = x^3 - 3; repeatedly a(k) = floor of the unique real root
 *            of P (found by sign tests), then P(x) <- x^3 * P(a(k) + 1/x).
 *            Exact big-integer arithmetic is used for the polynomial
 *            coefficients
 * Range:     u8 n <= 117, u16 n <= 9508, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *cf_C[4];
static uint32_t *cf_tmp;
static size_t cf_W = 0;
static uint32_t *cf_terms = NULL;
static idx_t cf_cnt = 0, cf_cap = 0;

static int cf_neg(const uint32_t *x) { return (x[cf_W - 1] >> 31) & 1; }

static void cf_mul(uint32_t *x, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < cf_W; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void cf_add(uint32_t *x, const uint32_t *y) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < cf_W; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void cf_init(void) {
    int k;
    cf_W = 8;
    for (k = 0; k < 4; k++) cf_C[k] = (uint32_t *)calloc(cf_W, sizeof(uint32_t));
    cf_tmp = (uint32_t *)calloc(cf_W, sizeof(uint32_t));
    cf_C[3][0] = 1;
    memset(cf_C[0], 0xFF, cf_W * sizeof(uint32_t));
    cf_C[0][0] = 0xFFFFFFFDu; /* -3 */
}

static void cf_ensure_room(void) {
    int k, need = 0;
    size_t i;
    for (k = 0; k < 4 && !need; k++) {
        uint32_t ext = cf_neg(cf_C[k]) ? 0xFFFFFFFFu : 0u;
        for (i = cf_W - 6; i < cf_W; i++)
            if (cf_C[k][i] != ext) { need = 1; break; }
    }
    if (need) {
        size_t nw = cf_W + 64;
        for (k = 0; k < 4; k++) {
            uint32_t ext = cf_neg(cf_C[k]) ? 0xFFFFFFFFu : 0u;
            cf_C[k] = (uint32_t *)realloc(cf_C[k], nw * sizeof(uint32_t));
            for (i = cf_W; i < nw; i++) cf_C[k][i] = ext;
        }
        cf_tmp = (uint32_t *)realloc(cf_tmp, nw * sizeof(uint32_t));
        cf_W = nw;
    }
}

/* sign (1 = negative) of P(a) */
static int cf_eval_neg(uint32_t a) {
    int k;
    memcpy(cf_tmp, cf_C[3], cf_W * sizeof(uint32_t));
    for (k = 2; k >= 0; k--) {
        cf_mul(cf_tmp, a);
        cf_add(cf_tmp, cf_C[k]);
    }
    return cf_neg(cf_tmp);
}

static uint32_t cf_step(void) {
    int s, i, j;
    uint64_t lo, hi;
    uint32_t a;
    uint32_t *t;
    cf_ensure_room();
    s = cf_neg(cf_C[3]);
    /* root > 1 always; P(x) has sign s for x > root */
    lo = 1;
    hi = 2;
    while (cf_eval_neg((uint32_t)hi) != s) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (cf_eval_neg((uint32_t)mid) == s) hi = mid; else lo = mid;
    }
    a = (uint32_t)lo;
    /* Taylor shift: P(x) <- P(x + a) */
    for (i = 0; i < 3; i++) {
        for (j = 2; j >= i; j--) {
            memcpy(cf_tmp, cf_C[j + 1], cf_W * sizeof(uint32_t));
            cf_mul(cf_tmp, a);
            cf_add(cf_C[j], cf_tmp);
        }
    }
    /* reverse: x^3 * P(1/x) */
    t = cf_C[0]; cf_C[0] = cf_C[3]; cf_C[3] = t;
    t = cf_C[1]; cf_C[1] = cf_C[2]; cf_C[2] = t;
    return a;
}

seq_t a(idx_t n) {
    if (cf_W == 0) cf_init();
    if (n >= cf_cap) {
        idx_t nc = n + 256;
        cf_terms = (uint32_t *)realloc(cf_terms, nc * sizeof(uint32_t));
        cf_cap = nc;
    }
    while (cf_cnt <= n) {
        cf_terms[cf_cnt] = cf_step();
        cf_cnt++;
    }
    return (seq_t)cf_terms[n];
}
