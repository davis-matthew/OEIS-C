/*
 * A006941  Expansion of Pi in base 8.
 * https://oeis.org/A006941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = n-th digit of Pi in base 8 (a(1) = 3 is the integer
 *            part); Pi is computed in binary fixed point by Machin's formula
 *            Pi = 16*arctan(1/5) - 4*arctan(1/239), and each octal digit is
 *            a(n) = 4*A004601(3n) + 2*A004601(3n+1) + 1*A004601(3n+2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *pi_limbs = NULL;   /* pi_limbs[0] = integer part, then fraction */
static size_t pi_len = 0;
static idx_t pi_cap = 0;            /* number of octal digits available */

static void fx_div(uint32_t *x, size_t len, uint32_t d) {
    uint64_t rem = 0;
    size_t j;
    for (j = 0; j < len; j++) {
        uint64_t cur = (rem << 32) | x[j];
        x[j] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void fx_add(uint32_t *x, const uint32_t *y, size_t len) {
    uint64_t carry = 0;
    size_t j = len;
    while (j-- > 0) {
        uint64_t s = (uint64_t)x[j] + y[j] + carry;
        x[j] = (uint32_t)s;
        carry = s >> 32;
    }
}

static void fx_sub(uint32_t *x, const uint32_t *y, size_t len) {
    int64_t borrow = 0;
    size_t j = len;
    while (j-- > 0) {
        int64_t s = (int64_t)x[j] - (int64_t)y[j] - borrow;
        if (s < 0) { s += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x[j] = (uint32_t)s;
    }
}

static void fx_mul(uint32_t *x, size_t len, uint32_t m) {
    uint64_t carry = 0;
    size_t j = len;
    while (j-- > 0) {
        uint64_t p = (uint64_t)x[j] * m + carry;
        x[j] = (uint32_t)p;
        carry = p >> 32;
    }
}

static int fx_zero(const uint32_t *x, size_t len) {
    size_t j;
    for (j = 0; j < len; j++) if (x[j]) return 0;
    return 1;
}

/* res = arctan(1/k) */
static void fx_atan_inv(uint32_t *res, size_t len, uint32_t k) {
    uint32_t *term = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t i;
    memset(res, 0, len * sizeof(uint32_t));
    term[0] = 1;
    fx_div(term, len, k);
    memcpy(res, term, len * sizeof(uint32_t));
    for (i = 1;; i++) {
        fx_div(term, len, k);
        fx_div(term, len, k);
        if (fx_zero(term, len)) break;
        memcpy(t2, term, len * sizeof(uint32_t));
        fx_div(t2, len, 2 * i + 1);
        if (i & 1) fx_sub(res, t2, len);
        else fx_add(res, t2, len);
    }
    free(term);
    free(t2);
}

static void compute_pi(idx_t digits) {
    size_t bits = (size_t)digits * 3 + 64;
    size_t len = 1 + bits / 32 + 6;
    uint32_t *a5 = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *a239 = (uint32_t *)calloc(len, sizeof(uint32_t));
    fx_atan_inv(a5, len, 5);
    fx_atan_inv(a239, len, 239);
    fx_mul(a5, len, 16);
    fx_mul(a239, len, 4);
    fx_sub(a5, a239, len);
    free(a239);
    if (pi_limbs) free(pi_limbs);
    pi_limbs = a5;
    pi_len = len;
    pi_cap = digits;
}

static unsigned frac_bit(size_t b) {
    size_t limb = 1 + b / 32;
    return (unsigned)((pi_limbs[limb] >> (31 - (b % 32))) & 1u);
}

seq_t a(idx_t n) {
    size_t b;
    unsigned d;
    if (n <= 1) return (seq_t)3;
    if (n > pi_cap) {
        idx_t want = n * 2;
        if (want < 1024) want = 1024;
        compute_pi(want);
    }
    b = (size_t)(n - 2) * 3;
    d = 4 * frac_bit(b) + 2 * frac_bit(b + 1) + frac_bit(b + 2);
    (void)pi_len;
    return (seq_t)d;
}
