/*
 * A004554  Expansion of sqrt(3) in base 9.
 * https://oeis.org/A004554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3) * 9^(n-1)) mod 9, computed by
 *            digit-by-digit base-9 square root of 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t *w;
    size_t len, cap;
} a4554_big;

static void a4554_reserve(a4554_big *b, size_t cap) {
    if (cap > b->cap) {
        size_t nc = b->cap ? b->cap : 16;
        while (nc < cap) nc *= 2;
        b->w = (uint32_t *)realloc(b->w, nc * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void a4554_trim(a4554_big *b) {
    while (b->len > 0 && b->w[b->len - 1] == 0) b->len--;
}

/* dst = src * m + add  (dst may equal src) */
static void a4554_mul_add(a4554_big *dst, const a4554_big *src, uint32_t m, uint32_t add) {
    size_t n = src->len, i;
    uint64_t carry = add;
    a4554_reserve(dst, n + 2);
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)src->w[i] * m + carry;
        dst->w[i] = (uint32_t)t;
        carry = t >> 32;
    }
    dst->len = n;
    while (carry) {
        dst->w[dst->len++] = (uint32_t)carry;
        carry >>= 32;
    }
    a4554_trim(dst);
}

static int a4554_cmp(const a4554_big *x, const a4554_big *y) {
    size_t i;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (i = x->len; i-- > 0;) {
        if (x->w[i] != y->w[i]) return x->w[i] < y->w[i] ? -1 : 1;
    }
    return 0;
}

/* x -= y, requires x >= y */
static void a4554_sub(a4554_big *x, const a4554_big *y) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < x->len; i++) {
        int64_t t = (int64_t)x->w[i] - borrow - (i < y->len ? (int64_t)y->w[i] : 0);
        if (t < 0) {
            t += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        x->w[i] = (uint32_t)t;
    }
    a4554_trim(x);
}

seq_t a(idx_t n) {
    static unsigned char *digits = NULL;
    static size_t done = 0, dcap = 0;
    static a4554_big p = {NULL, 0, 0}, r = {NULL, 0, 0};
    static a4554_big q = {NULL, 0, 0}, y = {NULL, 0, 0};

    if (n < 1) return 0;
    if ((size_t)n > dcap) {
        size_t nc = dcap ? dcap : 64;
        while (nc < (size_t)n) nc *= 2;
        digits = (unsigned char *)realloc(digits, nc);
        dcap = nc;
    }
    while (done < (size_t)n) {
        uint32_t pair = (done == 0) ? 3u : 0u;
        uint32_t d;
        a4554_mul_add(&r, &r, 81u, pair);
        a4554_mul_add(&q, &p, 18u, 0u);
        for (d = 8; d > 0; d--) {
            a4554_mul_add(&y, &q, d, d * d);
            if (a4554_cmp(&y, &r) <= 0) break;
        }
        if (d > 0) a4554_sub(&r, &y);
        a4554_mul_add(&p, &p, 9u, d);
        digits[done++] = (unsigned char)d;
    }
    return (seq_t)digits[n - 1];
}
