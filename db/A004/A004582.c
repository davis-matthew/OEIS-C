/*
 * A004582  Expansion of sqrt(8) in base 7.
 * https://oeis.org/A004582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8)*7^(n-1)) mod 7, computed via the
 *            digit-by-digit square root in base 7 with small-limb bignums
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A4582_B 1000000000u

typedef struct { uint32_t *d; size_t len, cap; } a4582_big;

static void a4582_ensure(a4582_big *x, size_t cap) {
    if (x->cap < cap) {
        size_t nc = cap * 2 + 8;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

/* dst = src * m + add  (dst may alias src) */
static void a4582_muladd(a4582_big *dst, const a4582_big *src, uint32_t m, uint32_t add) {
    size_t sl = src->len, i;
    uint64_t carry = add;
    a4582_ensure(dst, sl + 3);
    for (i = 0; i < sl; i++) {
        uint64_t t = (uint64_t)src->d[i] * m + carry;
        dst->d[i] = (uint32_t)(t % A4582_B);
        carry = t / A4582_B;
    }
    while (carry) {
        dst->d[i++] = (uint32_t)(carry % A4582_B);
        carry /= A4582_B;
    }
    while (i > 0 && dst->d[i - 1] == 0) i--;
    dst->len = i;
}

static int a4582_cmp(const a4582_big *x, const a4582_big *y) {
    size_t i;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (i = x->len; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

/* x -= y, requires x >= y */
static void a4582_sub(a4582_big *x, const a4582_big *y) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < x->len; i++) {
        int64_t t = (int64_t)x->d[i] - borrow - (i < y->len ? (int64_t)y->d[i] : 0);
        if (t < 0) { t += A4582_B; borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)t;
    }
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}

static unsigned char *a4582_dig = NULL;
static size_t a4582_cnt = 0, a4582_dcap = 0;
static a4582_big a4582_p = {NULL, 0, 0}, a4582_r = {NULL, 0, 0};
static a4582_big a4582_t1 = {NULL, 0, 0}, a4582_t2 = {NULL, 0, 0};

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a4582_cnt == 0) {
        /* p = floor(sqrt(8)) = 2, r = 8 - 4 = 4 */
        a4582_ensure(&a4582_p, 4);
        a4582_ensure(&a4582_r, 4);
        a4582_p.d[0] = 2; a4582_p.len = 1;
        a4582_r.d[0] = 4; a4582_r.len = 1;
        a4582_dcap = 64;
        a4582_dig = (unsigned char *)malloc(a4582_dcap);
        a4582_dig[0] = 2;
        a4582_cnt = 1;
    }
    while (a4582_cnt < (size_t)n) {
        uint32_t d;
        a4582_muladd(&a4582_r, &a4582_r, 49, 0);
        for (d = 6; d >= 1; d--) {
            a4582_muladd(&a4582_t1, &a4582_p, 14, d);
            a4582_muladd(&a4582_t2, &a4582_t1, d, 0);
            if (a4582_cmp(&a4582_t2, &a4582_r) <= 0) break;
        }
        if (d >= 1) a4582_sub(&a4582_r, &a4582_t2);
        a4582_muladd(&a4582_p, &a4582_p, 7, d);
        if (a4582_cnt >= a4582_dcap) {
            a4582_dcap *= 2;
            a4582_dig = (unsigned char *)realloc(a4582_dig, a4582_dcap);
        }
        a4582_dig[a4582_cnt++] = (unsigned char)d;
    }
    return (seq_t)a4582_dig[n - 1];
}
