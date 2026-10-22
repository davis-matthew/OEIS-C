/*
 * A005480  Decimal expansion of cube root of 4.
 * https://oeis.org/A005480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(4^(1/3) * 10^(n-1)) mod 10, computed by the
 *            digit-by-digit integer cube root algorithm (base-10^9 limb
 *            bignums, state kept between calls)
 * Range:     u8 n <= 9709, u16 n <= 9709, u32 n <= 9709, u64 n <= 9709,
 *            u128 n <= 9709, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BN_BASE 1000000000ULL

typedef struct { uint32_t *v; size_t len, cap; } bn;

static void bn_res(bn *b, size_t n) {
    if (n > b->cap) {
        size_t c = b->cap ? b->cap : 16;
        while (c < n) c *= 2;
        b->v = (uint32_t *)realloc(b->v, c * sizeof(uint32_t));
        b->cap = c;
    }
}

static void bn_trim(bn *b) {
    while (b->len && b->v[b->len - 1] == 0) b->len--;
}

/* dst = src * k  (dst may equal src) */
static void bn_mul_small(bn *dst, bn *src, uint32_t k) {
    size_t n = src->len, i;
    uint64_t c = 0;
    bn_res(dst, n + 1);
    for (i = 0; i < n; i++) {
        uint64_t x = (uint64_t)src->v[i] * k + c;
        dst->v[i] = (uint32_t)(x % BN_BASE);
        c = x / BN_BASE;
    }
    dst->len = n;
    if (c) dst->v[dst->len++] = (uint32_t)c;
    bn_trim(dst);
}

/* dst += a */
static void bn_add(bn *dst, const bn *a) {
    size_t m = dst->len > a->len ? dst->len : a->len, i;
    uint64_t c = 0;
    bn_res(dst, m + 1);
    for (i = dst->len; i < m; i++) dst->v[i] = 0;
    dst->len = m;
    for (i = 0; i < m; i++) {
        uint64_t x = (uint64_t)dst->v[i] + (i < a->len ? a->v[i] : 0) + c;
        if (x >= BN_BASE) { x -= BN_BASE; c = 1; } else c = 0;
        dst->v[i] = (uint32_t)x;
    }
    if (c) dst->v[dst->len++] = 1;
}

static void bn_add_small(bn *dst, uint32_t k) {
    size_t i = 0;
    uint64_t c = k;
    bn_res(dst, dst->len + 1);
    while (c) {
        uint64_t x;
        if (i == dst->len) dst->v[dst->len++] = 0;
        x = (uint64_t)dst->v[i] + c;
        dst->v[i] = (uint32_t)(x % BN_BASE);
        c = x / BN_BASE;
        i++;
    }
}

/* dst -= a, requires dst >= a */
static void bn_sub(bn *dst, const bn *a) {
    size_t i;
    int64_t br = 0;
    for (i = 0; i < dst->len; i++) {
        int64_t x = (int64_t)dst->v[i] - (i < a->len ? (int64_t)a->v[i] : 0) - br;
        if (x < 0) { x += (int64_t)BN_BASE; br = 1; } else br = 0;
        dst->v[i] = (uint32_t)x;
    }
    bn_trim(dst);
}

static int bn_cmp(const bn *a, const bn *b) {
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (i = a->len; i-- > 0;) {
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    }
    return 0;
}

static bn st_y, st_y2, st_r, st_A, st_B, st_t;
static unsigned char *digs = NULL;
static size_t ndig = 0, capdig = 0;

static void eval_t(uint32_t d) {
    /* t = d*(A + B*d) + d^3 */
    bn_mul_small(&st_t, &st_B, d);
    bn_add(&st_t, &st_A);
    bn_mul_small(&st_t, &st_t, d);
    bn_add_small(&st_t, d * d * d);
}

static void next_digit(void) {
    uint32_t block = (ndig == 0) ? 4 : 0;
    uint32_t lo = 0, hi = 9, d;

    bn_mul_small(&st_r, &st_r, 1000);
    bn_add_small(&st_r, block);

    bn_mul_small(&st_A, &st_y2, 300);
    bn_mul_small(&st_B, &st_y, 30);

    while (lo < hi) {
        uint32_t mid = (lo + hi + 1) / 2;
        eval_t(mid);
        if (bn_cmp(&st_t, &st_r) <= 0) lo = mid; else hi = mid - 1;
    }
    d = lo;
    eval_t(d);
    bn_sub(&st_r, &st_t);

    /* y2 = 100*y2 + 20*d*y + d^2 */
    bn_mul_small(&st_t, &st_y, 20 * d);
    bn_mul_small(&st_y2, &st_y2, 100);
    bn_add(&st_y2, &st_t);
    bn_add_small(&st_y2, d * d);

    /* y = 10*y + d */
    bn_mul_small(&st_y, &st_y, 10);
    bn_add_small(&st_y, d);

    if (ndig == capdig) {
        capdig = capdig ? capdig * 2 : 256;
        digs = (unsigned char *)realloc(digs, capdig);
    }
    digs[ndig++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    while (ndig < (size_t)n) next_digit();
    return (seq_t)digs[n - 1];
}
