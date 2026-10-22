/*
 * A004565  Expansion of sqrt(6) in base 6.
 * https://oeis.org/A004565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6) * 6^(n-1)) mod 6, i.e. the n-th base-6
 *            digit of sqrt(6), computed with the digit-by-digit square-root
 *            method on 32-bit-limb bignums
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *w; size_t len, cap; } a004565_big;

static void a004565_reserve(a004565_big *b, size_t c) {
    if (c > b->cap) {
        size_t nc = c * 2 + 8, i;
        b->w = (uint32_t *)realloc(b->w, nc * sizeof(uint32_t));
        for (i = b->cap; i < nc; i++) b->w[i] = 0;
        b->cap = nc;
    }
}

static void a004565_trim(a004565_big *b) {
    while (b->len > 0 && b->w[b->len - 1] == 0) b->len--;
}

/* b = b*m + a */
static void a004565_muladd(a004565_big *b, uint32_t m, uint32_t a) {
    uint64_t c = a;
    size_t i;
    for (i = 0; i < b->len; i++) {
        c += (uint64_t)b->w[i] * m;
        b->w[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) {
        a004565_reserve(b, b->len + 1);
        b->w[b->len++] = (uint32_t)c;
    }
    a004565_trim(b);
}

static void a004565_copy(a004565_big *d, const a004565_big *s) {
    a004565_reserve(d, s->len + 1);
    if (s->len) memcpy(d->w, s->w, s->len * sizeof(uint32_t));
    d->len = s->len;
}

static int a004565_cmp(const a004565_big *a, const a004565_big *b) {
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (i = a->len; i-- > 0;) {
        if (a->w[i] != b->w[i]) return a->w[i] < b->w[i] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void a004565_sub(a004565_big *a, const a004565_big *b) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->len; i++) {
        int64_t v = (int64_t)a->w[i] - br - (i < b->len ? (int64_t)b->w[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->w[i] = (uint32_t)v;
    }
    a004565_trim(a);
}

static a004565_big a004565_P, a004565_R, a004565_T;
static unsigned char *a004565_dig = NULL;
static size_t a004565_ndig = 0, a004565_dcap = 0;

seq_t a(idx_t n) {
    size_t need;
    if (n < 1) return (seq_t)0;
    need = (size_t)n;
    if (need > a004565_dcap) {
        size_t nc = need * 2 + 16;
        a004565_dig = (unsigned char *)realloc(a004565_dig, nc);
        a004565_dcap = nc;
    }
    while (a004565_ndig < need) {
        uint32_t pair = (a004565_ndig == 0) ? 6u : 0u;
        int d;
        a004565_muladd(&a004565_R, 36u, pair);
        for (d = 5; d >= 0; d--) {
            a004565_copy(&a004565_T, &a004565_P);
            a004565_muladd(&a004565_T, 12u, (uint32_t)d);
            a004565_muladd(&a004565_T, (uint32_t)d, 0u);
            if (a004565_cmp(&a004565_T, &a004565_R) <= 0) break;
        }
        if (d < 0) d = 0;
        a004565_sub(&a004565_R, &a004565_T);
        a004565_muladd(&a004565_P, 6u, (uint32_t)d);
        a004565_dig[a004565_ndig++] = (unsigned char)d;
    }
    return (seq_t)a004565_dig[n - 1];
}
