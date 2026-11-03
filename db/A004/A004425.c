/*
 * A004425  Expansion of (Sum x^(n^2), n = -inf .. inf )^(-24).
 * https://oeis.org/A004425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   n*a(n) = Sum_{k>=1, k^2<=n} 2*(-23*k^2 - n)*a(n-k^2), a(0) = 1
 *            (J.C.P. Miller power recurrence for theta(x)^(-24), theta(x) = 1
 *            + 2*Sum_{k>=1} x^(k^2))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 21, i128 n <= 62,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 9523 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

/* arbitrary-precision path: cache of seq_t values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* fixed-width path: exact __int128 values, stopped before overflow */
static __int128 *c128 = NULL;
static idx_t c128_cap = 0;
static idx_t c128_len = 0;
static bool c128_done = false;

static seq_t a_fixed(idx_t n) {
    if (c128 == NULL) {
        c128_cap = 64;
        c128 = (__int128 *)malloc(c128_cap * sizeof(__int128));
        c128[0] = 1;
        c128_len = 1;
    }
    const __int128 limit = ((__int128)1) << 100;
    while (!c128_done && c128_len <= n) {
        idx_t i = c128_len;
        if (i >= c128_cap) {
            c128_cap *= 2;
            c128 = (__int128 *)realloc(c128, c128_cap * sizeof(__int128));
        }
        __int128 s = 0;
        for (idx_t j = 1; j * j <= i; j++) {
            idx_t k = j * j;
            s -= (__int128)2 * ((__int128)23 * (__int128)k + (__int128)i) * c128[i - k];
        }
        __int128 v = s / (__int128)i;
        c128[i] = v;
        c128_len = i + 1;
        if (v > limit || v < -limit) c128_done = true;
    }
    if (n < c128_len) return from_i128(c128[n]);
    return 0; /* far beyond any fixed-width seq_t range */
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);

    if (cache == NULL) {
        cache_cap = 64;
        cache = alloc_zero(cache_cap);
        cache[0] = 1;
        cache_len = 1;
    }
    if (n >= cache_cap) {
        idx_t newcap = cache_cap;
        while (newcap <= n) newcap *= 2;
        cache = grow_zero(cache, cache_cap, newcap);
        cache_cap = newcap;
    }
    for (idx_t i = cache_len; i <= n; i++) {
        seq_t s = 0;
        for (idx_t j = 1; j * j <= i; j++) {
            idx_t k = j * j;
            seq_t coef = ((seq_t)(23 * k) + (seq_t)i) * 2;
            s -= coef * cache[i - k];
        }
        cache[i] = s / (seq_t)i;
        cache_len = i + 1;
    }
    return cache[n];
}
