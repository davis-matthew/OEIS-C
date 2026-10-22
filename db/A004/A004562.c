/*
 * A004562  Expansion of sqrt(5) in base 9.
 * https://oeis.org/A004562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 9^(n-1)) mod 9; computed by the
 *            digit-by-digit (base 9) square root algorithm on exact big
 *            integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct bign_s {
    uint32_t *d;
    size_t len, cap;
} bign;

static void bn_reserve(bign *a, size_t n) {
    if (a->cap < n) {
        size_t c = a->cap ? a->cap : 16;
        while (c < n) c *= 2;
        a->d = (uint32_t *)realloc(a->d, c * sizeof(uint32_t));
        a->cap = c;
    }
}

static void bn_trim(bign *a) {
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

static void bn_set_small(bign *a, uint32_t v) {
    bn_reserve(a, 1);
    a->d[0] = v;
    a->len = v ? 1 : 0;
}

static void bn_copy(bign *dst, const bign *src) {
    bn_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

/* a = a*m + add */
static void bn_mul_add_small(bign *a, uint32_t m, uint32_t add) {
    uint64_t c = add;
    size_t i;
    for (i = 0; i < a->len; i++) {
        c += (uint64_t)a->d[i] * m;
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) {
        bn_reserve(a, a->len + 1);
        a->d[a->len++] = (uint32_t)c;
    }
    bn_trim(a);
}

/* dst = a + b (dst distinct from a and b) */
static void bn_add(bign *dst, const bign *a, const bign *b) {
    size_t n = a->len > b->len ? a->len : b->len;
    size_t i;
    uint64_t c = 0;
    bn_reserve(dst, n + 1);
    for (i = 0; i < n; i++) {
        c += (i < a->len ? a->d[i] : 0);
        c += (i < b->len ? b->d[i] : 0);
        dst->d[i] = (uint32_t)c;
        c >>= 32;
    }
    dst->len = n;
    if (c) dst->d[dst->len++] = (uint32_t)c;
}

static int bn_cmp(const bign *a, const bign *b) {
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (i = a->len; i > 0; i--) {
        if (a->d[i - 1] != b->d[i - 1]) return a->d[i - 1] < b->d[i - 1] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void bn_sub(bign *a, const bign *b) {
    size_t i;
    int64_t br = 0;
    for (i = 0; i < a->len; i++) {
        int64_t v = (int64_t)a->d[i] - br - (int64_t)(i < b->len ? b->d[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)v;
    }
    bn_trim(a);
}

static bign R, P, T, S, NX;
static unsigned char *digits = NULL;
static size_t ndig = 0, dcap = 0;
static int inited = 0;

static void extend_to(size_t need) {
    if (!inited) {
        bn_set_small(&R, 5);
        bn_set_small(&P, 0);
        inited = 1;
    }
    if (dcap < need) {
        size_t c = dcap ? dcap : 64;
        while (c < need) c *= 2;
        digits = (unsigned char *)realloc(digits, c);
        dcap = c;
    }
    while (ndig < need) {
        uint32_t d = 0;
        if (ndig > 0) bn_mul_add_small(&R, 81, 0);
        bn_copy(&T, &P);
        bn_mul_add_small(&T, 18, 0);
        bn_set_small(&S, 0);
        while (d < 8) {
            bign tmp;
            bn_add(&NX, &S, &T);
            bn_mul_add_small(&NX, 1, 2 * d + 1);
            if (bn_cmp(&NX, &R) > 0) break;
            tmp = S; S = NX; NX = tmp;
            d++;
        }
        bn_sub(&R, &S);
        bn_mul_add_small(&P, 9, d);
        digits[ndig++] = (unsigned char)d;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if ((size_t)n > ndig) extend_to((size_t)n);
    return (seq_t)digits[n - 1];
}
