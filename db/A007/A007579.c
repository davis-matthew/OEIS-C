/*
 * A007579  Number of Young tableaux of height <= 6.
 * https://oeis.org/A007579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+5)*(n+8)*(n+9)*a(n) = 4*(5*n^2+46*n+84)*a(n-1) +
 *            4*(n-1)*(10*n^2+58*n+33)*a(n-2) - 144*(n-1)*(n-2)*a(n-3) -
 *            144*(n-1)*(n-2)*(n-3)*a(n-4), a(0) = 1, a(1) = 1, a(2) = 2, a(3)
 *            = 4
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 60,
 *            big any n
 * Helpers:   seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

typedef struct { uint32_t *d; size_t len, cap; } big_t;

static void bg_reserve(big_t *b, size_t c) {
    if (c > b->cap) {
        size_t nc = c * 2 + 8;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        if (b->d == NULL) abort();
        b->cap = nc;
    }
}

static void bg_trim(big_t *b) {
    while (b->len && b->d[b->len - 1] == 0) b->len--;
}

static void bg_set_small(big_t *b, uint32_t v) {
    bg_reserve(b, 1);
    b->d[0] = v;
    b->len = 1;
    bg_trim(b);
}

/* dst = src * m */
static void bg_mul(big_t *dst, const big_t *src, uint64_t m) {
    size_t k;
    unsigned __int128 carry = 0;
    bg_reserve(dst, src->len + 4);
    for (k = 0; k < src->len; k++) {
        unsigned __int128 cur = (unsigned __int128)src->d[k] * m + carry;
        dst->d[k] = (uint32_t)cur;
        carry = cur >> 32;
    }
    while (carry) {
        dst->d[k++] = (uint32_t)carry;
        carry >>= 32;
    }
    dst->len = k;
    bg_trim(dst);
}

/* dst += src */
static void bg_add(big_t *dst, const big_t *src) {
    size_t mx = dst->len > src->len ? dst->len : src->len;
    size_t k;
    uint64_t carry = 0;
    bg_reserve(dst, mx + 1);
    for (k = 0; k < mx; k++) {
        uint64_t s = carry;
        if (k < dst->len) s += dst->d[k];
        if (k < src->len) s += src->d[k];
        dst->d[k] = (uint32_t)s;
        carry = s >> 32;
    }
    if (carry) dst->d[k++] = (uint32_t)carry;
    dst->len = k;
}

/* dst -= src, requires dst >= src */
static void bg_sub(big_t *dst, const big_t *src) {
    size_t k;
    int64_t borrow = 0;
    for (k = 0; k < dst->len; k++) {
        int64_t s = (int64_t)dst->d[k] - borrow - (int64_t)(k < src->len ? src->d[k] : 0);
        if (s < 0) { s += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        dst->d[k] = (uint32_t)s;
    }
    bg_trim(dst);
}

/* b /= d, exact */
static void bg_div(big_t *b, uint32_t d) {
    uint64_t rem = 0;
    size_t k = b->len;
    while (k > 0) {
        k--;
        rem = (rem << 32) | b->d[k];
        b->d[k] = (uint32_t)(rem / d);
        rem %= d;
    }
    bg_trim(b);
}

static seq_t bg_to_seq(const big_t *b) {
    size_t k;
    if (seq_is_big()) {
        seq_t v = 0;
        seq_t base = (seq_t)65536;
        k = b->len;
        while (k > 0) {
            k--;
            v = v * base * base + (seq_t)(uint64_t)b->d[k];
        }
        return v;
    } else {
        unsigned __int128 v = 0;
        k = b->len < 4 ? b->len : 4;
        while (k > 0) {
            k--;
            v = (v << 32) | b->d[k];
        }
        return u128_to_seq(v);
    }
}

static big_t *M = NULL;
static idx_t mcap = 0;
static idx_t done = 0;          /* M[0..done-1] valid */

seq_t a(idx_t n) {
    if (M == NULL) {
        idx_t j;
        mcap = 16;
        M = (big_t *)calloc(mcap, sizeof(big_t));
        if (M == NULL) abort();
        bg_set_small(&M[0], 1);
        bg_set_small(&M[1], 1);
        bg_set_small(&M[2], 2);
        bg_set_small(&M[3], 4);
        done = 4;
        (void)j;
    }
    if (n >= mcap) {
        idx_t nc = mcap, j;
        big_t *nm;
        while (nc <= n) nc *= 2;
        nm = (big_t *)realloc(M, nc * sizeof(big_t));
        if (nm == NULL) abort();
        for (j = mcap; j < nc; j++) { nm[j].d = NULL; nm[j].len = 0; nm[j].cap = 0; }
        M = nm;
        mcap = nc;
    }
    if (n >= done) {
        idx_t i;
        big_t T2;
        T2.d = NULL; T2.len = 0; T2.cap = 0;
        for (i = done; i <= n; i++) {
            uint64_t c1 = 4 * (5 * i * i + 46 * i + 84);
            uint64_t c2 = 4 * (i - 1) * (10 * i * i + 58 * i + 33);
            uint64_t c3 = 144 * (i - 1) * (i - 2);
            uint64_t c4 = c3 * (i - 3);
            big_t *T1 = &M[i];

            bg_mul(T1, &M[i - 1], c1);
            bg_mul(&T2, &M[i - 2], c2);
            bg_add(T1, &T2);
            bg_mul(&T2, &M[i - 3], c3);
            bg_sub(T1, &T2);
            bg_mul(&T2, &M[i - 4], c4);
            bg_sub(T1, &T2);
            bg_div(T1, (uint32_t)(i + 5));
            bg_div(T1, (uint32_t)(i + 8));
            bg_div(T1, (uint32_t)(i + 9));
        }
        free(T2.d);
        done = n + 1;
    }
    return bg_to_seq(&M[n]);
}
