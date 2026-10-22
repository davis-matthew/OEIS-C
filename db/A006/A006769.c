/*
 * A006769  Elliptic divisibility sequence associated with elliptic curve
 *          "37a1": y^2 + y = x^3 - x and multiples of the point (0,0).
 * https://oeis.org/A006769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = (a(n-1)*a(n-3) + a(n-2)^2) / a(n-4), a(0) = 0, a(1) = 1,
 *            a(2) = 1, a(3) = -1, a(4) = 1 (fixed-width builds evaluate the
 *            products in exact 256-bit arithmetic)
 * Range:     i8 n <= 13, i16 n <= 20, i32 n <= 29, i64 n <= 42, i128 n <= 58,
 *            big any n
 * Helpers:   from_i128, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 u128_t;

static void mul_u128(u128_t x, u128_t y, u128_t *hi, u128_t *lo) {
    const u128_t M = (u128_t)UINT64_MAX;
    u128_t x0 = x & M, x1 = x >> 64, y0 = y & M, y1 = y >> 64;
    u128_t p00 = x0 * y0, p01 = x0 * y1, p10 = x1 * y0, p11 = x1 * y1;
    u128_t mid = (p00 >> 64) + (p01 & M) + (p10 & M);
    *lo = (p00 & M) | (mid << 64);
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

static u128_t mag_i128(__int128 v) {
    return v < 0 ? (u128_t)0 - (u128_t)v : (u128_t)v;
}

/* exact (x*y + z*z) / w, assuming the quotient fits in 128 bits */
static __int128 somos_step(__int128 x, __int128 y, __int128 z, __int128 w) {
    u128_t ah, al, bh, bl, h, l, d, rem, q;
    int sa, neg, i;
    if (w == 0) return 0;
    sa = ((x < 0) != (y < 0)) && x != 0 && y != 0;
    mul_u128(mag_i128(x), mag_i128(y), &ah, &al);
    mul_u128(mag_i128(z), mag_i128(z), &bh, &bl);
    if (!sa) {
        l = al + bl;
        h = ah + bh + (l < al ? 1 : 0);
        neg = 0;
    } else if (ah > bh || (ah == bh && al >= bl)) {
        l = al - bl;
        h = ah - bh - (al < bl ? 1 : 0);
        neg = 1;
    } else {
        l = bl - al;
        h = bh - ah - (bl < al ? 1 : 0);
        neg = 0;
    }
    if (w < 0) neg = !neg;
    d = mag_i128(w);
    rem = 0;
    q = 0;
    for (i = 255; i >= 0; i--) {
        u128_t bit = (i >= 128) ? ((h >> (i - 128)) & 1) : ((l >> i) & 1);
        int carry = (int)(rem >> 127);
        rem = (rem << 1) | bit;
        q <<= 1;
        if (carry || rem >= d) {
            rem -= d;
            q |= 1;
        }
    }
    return neg ? (__int128)((u128_t)0 - q) : (__int128)q;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        static __int128 *c = NULL;
        static idx_t sz = 0, cap = 0;
        idx_t i;
        if (c == NULL) {
            cap = 32;
            c = (__int128 *)malloc(cap * sizeof(__int128));
            c[0] = 0; c[1] = 1; c[2] = 1; c[3] = -1; c[4] = 1;
            sz = 5;
        }
        if (n >= cap) {
            idx_t nc = (n + 1) * 2;
            c = (__int128 *)realloc(c, nc * sizeof(__int128));
            cap = nc;
        }
        for (i = sz; i <= n; i++)
            c[i] = somos_step(c[i-1], c[i-3], c[i-2], c[i-4]);
        if (n + 1 > sz) sz = n + 1;
        return from_i128(c[n]);
    } else {
        static seq_t *cache = NULL;
        static idx_t cache_size = 0;
        static idx_t cache_cap = 0;
        idx_t i;
        if (cache == NULL) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 0; cache[1] = 1; cache[2] = 1; cache[3] = -1; cache[4] = 1;
            cache_size = 5;
        }
        if (n < cache_size) return cache[n];
        if (n >= cache_cap) {
            idx_t new_cap = (n + 1) * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (i = cache_size; i <= n; i++) {
            seq_t num = cache[i-1] * cache[i-3] + cache[i-2] * cache[i-2];
            cache[i] = num / cache[i-4];
        }
        cache_size = n + 1;
        return cache[n];
    }
}
