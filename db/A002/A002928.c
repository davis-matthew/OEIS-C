/*
 * A002928  Magnetization for square lattice.
 * https://oeis.org/A002928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   n*a(n) = 6*(n-1)*a(n-1) - 4*a(n-2) - 6*(n-3)*a(n-3) +
 *            (n-4)*a(n-4), a(0) = 1, a(1) = 0, a(2) = -2, a(3) = -8.
 *            Rewritten as a(n) = 6*a(n-1) - 6*a(n-3) + a(n-4) + R/n with R =
 *            -6*a(n-1) - 4*a(n-2) + 18*a(n-3) - 4*a(n-4), so intermediates
 *            stay small
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>

seq_t a(idx_t n) {
    static std::vector<seq_t> c;
    if (c.empty()) {
        c.push_back((seq_t)1);
        c.push_back((seq_t)0);
        c.push_back((seq_t)-2);
        c.push_back((seq_t)-8);
    }
    while ((idx_t)c.size() <= n) {
        idx_t i = (idx_t)c.size();
        seq_t x1 = c[i - 1], x2 = c[i - 2], x3 = c[i - 3], x4 = c[i - 4];
        seq_t R = -6 * x1 - 4 * x2 + 18 * x3 - 4 * x4;
        seq_t v = 6 * x1 - 6 * x3 + x4 + R / (seq_t)i;
        c.push_back(v);
    }
    return c[n];
}

#else

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_n = 0; /* number of computed terms */

seq_t a(idx_t n) {
    typedef unsigned __int128 u128;
    typedef __int128 s128;
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)-8;
        cache_n = 4;
    }
    if (n >= cache_cap) {
        idx_t nc = cache_cap;
        while (nc <= n) nc *= 2;
        cache = (seq_t *)realloc(cache, nc * sizeof(seq_t));
        cache_cap = nc;
    }
    while (cache_n <= n) {
        idx_t i = cache_n;
        s128 I = (s128)i;
        s128 x1 = (s128)cache[i - 1], x2 = (s128)cache[i - 2];
        s128 x3 = (s128)cache[i - 3], x4 = (s128)cache[i - 4];
        s128 q1 = x1 / I, r1 = x1 % I;
        s128 q2 = x2 / I, r2 = x2 % I;
        s128 q3 = x3 / I, r3 = x3 % I;
        s128 q4 = x4 / I, r4 = x4 % I;
        s128 rs = -6 * r1 - 4 * r2 + 18 * r3 - 4 * r4;
        /* wrapping arithmetic modulo 2^128; exact when the result fits */
        u128 u = (u128)6 * (u128)x1 - (u128)6 * (u128)x3 + (u128)x4;
        u += (u128)0 - (u128)6 * (u128)q1;
        u += (u128)0 - (u128)4 * (u128)q2;
        u += (u128)18 * (u128)q3;
        u += (u128)0 - (u128)4 * (u128)q4;
        u += (u128)(rs / I);
        cache[i] = (seq_t)(s128)u;
        cache_n++;
    }
    return cache[n];
}

#endif
