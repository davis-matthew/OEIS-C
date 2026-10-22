/*
 * A004548  Expansion of sqrt(3) in base 3.
 * https://oeis.org/A004548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3^(2n-1))) mod 3, i.e. the n-th base-3 digit
 *            of sqrt(3). Computed with the digit-by-digit base-3 square root:
 *            x(1) = 1, r(1) = 2, x(k) = 3*x(k-1) + d, r(k) = 9*r(k-1) -
 *            6*x(k-1)*d - d^2, where d is the largest value in {0,1,2} that
 *            keeps r(k) >= 0, and a(k) = d
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t *v;
    size_t len;
    size_t cap;
} bn_t;

static void bn_reserve(bn_t *b, size_t need) {
    if (need > b->cap) {
        size_t nc = b->cap ? b->cap : 16;
        while (nc < need) nc *= 2;
        b->v = (uint32_t *)realloc(b->v, nc * sizeof(uint32_t));
        memset(b->v + b->cap, 0, (nc - b->cap) * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void bn_trim(bn_t *b) {
    while (b->len > 0 && b->v[b->len - 1] == 0) b->len--;
}

/* dst = src * m + add  (dst may alias src) */
static void bn_muladd(bn_t *dst, const bn_t *src, uint32_t m, uint32_t add) {
    size_t n = src->len;
    bn_reserve(dst, n + 2);
    uint64_t carry = add;
    for (size_t i = 0; i < n; i++) {
        uint64_t t = (uint64_t)src->v[i] * m + carry;
        dst->v[i] = (uint32_t)t;
        carry = t >> 32;
    }
    dst->len = n;
    if (carry) {
        dst->v[n] = (uint32_t)carry;
        dst->len = n + 1;
    }
    bn_trim(dst);
}

static int bn_cmp(const bn_t *a, const bn_t *b) {
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void bn_sub(bn_t *a, const bn_t *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        int64_t t = (int64_t)a->v[i] - borrow - (i < b->len ? (int64_t)b->v[i] : 0);
        if (t < 0) {
            t += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->v[i] = (uint32_t)t;
    }
    bn_trim(a);
}

static bn_t sx = {NULL, 0, 0};
static bn_t sr = {NULL, 0, 0};
static bn_t st = {NULL, 0, 0};
static unsigned char *digits = NULL;
static idx_t ndigits = 0;
static idx_t dcap = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (ndigits == 0) {
        bn_reserve(&sx, 16);
        bn_reserve(&sr, 16);
        bn_reserve(&st, 16);
        sx.v[0] = 1; sx.len = 1;
        sr.v[0] = 2; sr.len = 1;
        dcap = 64;
        digits = (unsigned char *)malloc((size_t)dcap);
        digits[1] = 1;
        ndigits = 1;
    }
    if (n > ndigits) {
        if (n + 1 > dcap) {
            idx_t nc = dcap;
            while (nc < n + 1) nc *= 2;
            digits = (unsigned char *)realloc(digits, (size_t)nc);
            dcap = nc;
        }
        for (idx_t k = ndigits + 1; k <= n; k++) {
            unsigned d;
            bn_muladd(&sr, &sr, 9, 0);
            bn_muladd(&st, &sx, 12, 4);
            if (bn_cmp(&sr, &st) >= 0) {
                d = 2;
                bn_sub(&sr, &st);
            } else {
                bn_muladd(&st, &sx, 6, 1);
                if (bn_cmp(&sr, &st) >= 0) {
                    d = 1;
                    bn_sub(&sr, &st);
                } else {
                    d = 0;
                }
            }
            bn_muladd(&sx, &sx, 3, d);
            digits[k] = (unsigned char)d;
        }
        ndigits = n;
    }
    return (seq_t)digits[n];
}
