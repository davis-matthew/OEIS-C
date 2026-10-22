/*
 * A004572  Expansion of sqrt(7) in base 5.
 * https://oeis.org/A004572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(7) * 5^(n-1)) mod 5, i.e. the n-th base-5
 *            digit of sqrt(7); computed by the exact digit-by-digit square
 *            root algorithm with native multi-limb integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *v; size_t len, cap; } bnum;

static void bn_reserve(bnum *b, size_t c) {
    if (c > b->cap) {
        size_t nc = c * 2 + 8;
        b->v = (uint32_t *)realloc(b->v, nc * sizeof(uint32_t));
        memset(b->v + b->cap, 0, (nc - b->cap) * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void bn_set_small(bnum *b, uint32_t x) {
    bn_reserve(b, 1);
    b->len = 0;
    if (x) { b->v[0] = x; b->len = 1; }
}

/* b = b * m + a */
static void bn_mul_add(bnum *b, uint32_t m, uint32_t a) {
    uint64_t carry = a;
    size_t i;
    for (i = 0; i < b->len; i++) {
        uint64_t t = (uint64_t)b->v[i] * m + carry;
        b->v[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        bn_reserve(b, b->len + 1);
        b->v[b->len++] = (uint32_t)carry;
    }
    while (b->len && b->v[b->len - 1] == 0) b->len--;
}

static void bn_copy(bnum *dst, const bnum *src) {
    bn_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->v, src->v, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static int bn_cmp(const bnum *x, const bnum *y) {
    size_t i;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (i = x->len; i-- > 0;) {
        if (x->v[i] != y->v[i]) return x->v[i] < y->v[i] ? -1 : 1;
    }
    return 0;
}

/* x -= y, requires x >= y */
static void bn_sub(bnum *x, const bnum *y) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < x->len; i++) {
        int64_t t = (int64_t)x->v[i] - borrow - (i < y->len ? (int64_t)y->v[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x->v[i] = (uint32_t)t;
    }
    while (x->len && x->v[x->len - 1] == 0) x->len--;
}

static unsigned char *A004572_dig = NULL;
static idx_t A004572_have = 0, A004572_cap = 0;
static bnum A004572_x = {NULL, 0, 0}, A004572_r = {NULL, 0, 0}, A004572_t = {NULL, 0, 0};

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A004572_have == 0) {
        bn_set_small(&A004572_x, 2);
        bn_set_small(&A004572_r, 3);
        A004572_cap = 64;
        A004572_dig = (unsigned char *)malloc((size_t)A004572_cap);
        A004572_dig[1] = 2;
        A004572_have = 1;
    }
    if (n >= A004572_cap) {
        idx_t nc = n * 2 + 2;
        A004572_dig = (unsigned char *)realloc(A004572_dig, (size_t)nc);
        A004572_cap = nc;
    }
    while (A004572_have < n) {
        uint32_t d, chosen = 0;
        bn_mul_add(&A004572_r, 25, 0);
        for (d = 4; d >= 1; d--) {
            bn_copy(&A004572_t, &A004572_x);
            bn_mul_add(&A004572_t, 10, d);
            bn_mul_add(&A004572_t, d, 0);
            if (bn_cmp(&A004572_t, &A004572_r) <= 0) { chosen = d; break; }
        }
        if (chosen) bn_sub(&A004572_r, &A004572_t);
        bn_mul_add(&A004572_x, 5, chosen);
        A004572_have++;
        A004572_dig[A004572_have] = (unsigned char)chosen;
    }
    return (seq_t)A004572_dig[n];
}
