/*
 * A004592  Expansion of sqrt(10) in base 9.
 * https://oeis.org/A004592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(sqrt(10)*9^(n-1)) mod 9, computed by the
 *            digit-by-digit square root method in base 9 with native
 *            multi-limb integers, memoized
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t *d;
    size_t len, cap;
} a4592_big;

static void a4592_reserve(a4592_big *x, size_t cap) {
    if (cap > x->cap) {
        size_t nc = cap * 2 + 16;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        memset(x->d + x->cap, 0, (nc - x->cap) * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a4592_trim(a4592_big *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

/* dst = src * m + add  (dst may alias src) */
static void a4592_mulsmall(a4592_big *dst, a4592_big *src, uint32_t m, uint32_t add) {
    size_t n = src->len, i;
    uint64_t carry = add;
    a4592_reserve(dst, n + 1);
    for (i = 0; i < n; i++) {
        uint64_t v = (uint64_t)src->d[i] * m + carry;
        dst->d[i] = (uint32_t)v;
        carry = v >> 32;
    }
    dst->d[n] = (uint32_t)carry;
    dst->len = n + 1;
    a4592_trim(dst);
}

static int a4592_cmp(const a4592_big *x, const a4592_big *y) {
    size_t i;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (i = x->len; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

/* x -= y, requires x >= y */
static void a4592_sub(a4592_big *x, const a4592_big *y) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < x->len; i++) {
        int64_t v = (int64_t)x->d[i] - borrow - (i < y->len ? (int64_t)y->d[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)v;
    }
    a4592_trim(x);
}

static a4592_big a4592_p = {NULL, 0, 0};
static a4592_big a4592_r = {NULL, 0, 0};
static a4592_big a4592_u = {NULL, 0, 0};
static a4592_big a4592_t = {NULL, 0, 0};
static unsigned char *a4592_digits = NULL;
static idx_t a4592_count = 0, a4592_dcap = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a4592_count == 0) {
        a4592_reserve(&a4592_p, 4);
        a4592_reserve(&a4592_r, 4);
        a4592_p.d[0] = 3; a4592_p.len = 1;   /* sqrt(10) = 3.xxx */
        a4592_r.d[0] = 1; a4592_r.len = 1;   /* 10 - 3*3 */
        a4592_dcap = 64;
        a4592_digits = (unsigned char *)malloc((size_t)a4592_dcap);
        a4592_digits[1] = 3;
        a4592_count = 1;
    }
    if (n >= a4592_dcap) {
        idx_t nc = n * 2 + 16;
        a4592_digits = (unsigned char *)realloc(a4592_digits, (size_t)nc);
        a4592_dcap = nc;
    }
    while (a4592_count < n) {
        uint32_t lo = 0, hi = 8;
        a4592_mulsmall(&a4592_r, &a4592_r, 81, 0);
        a4592_mulsmall(&a4592_u, &a4592_p, 18, 0);
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            a4592_mulsmall(&a4592_t, &a4592_u, mid, mid * mid);
            if (a4592_cmp(&a4592_t, &a4592_r) <= 0) lo = mid; else hi = mid - 1;
        }
        a4592_mulsmall(&a4592_t, &a4592_u, lo, lo * lo);
        a4592_sub(&a4592_r, &a4592_t);
        a4592_mulsmall(&a4592_p, &a4592_p, 9, lo);
        a4592_count++;
        a4592_digits[a4592_count] = (unsigned char)lo;
    }
    return (seq_t)a4592_digits[n];
}
