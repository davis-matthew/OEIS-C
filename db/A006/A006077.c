/*
 * A006077  (n+1)^2*a(n+1) = (9n^2+9n+3)*a(n) - 27*n^2*a(n-1), with a(0) = 1
 *          and a(1) = 3.
 * https://oeis.org/A006077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   (n+1)^2*a(n+1) = (9n^2+9n+3)*a(n) - 27*n^2*a(n-1), a(0) = 1,
 *            a(1) = 3
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 16, i64 n <= 29, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 1259 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
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

#define SMALL_MAX 128

static __int128 small_cache[SMALL_MAX];
static int small_valid = -1; /* highest index computed exactly in __int128 */
static bool small_done = false;

static void fill_small(void) {
    __int128 cap = (__int128)1 << 100;
    int i;
    small_cache[0] = 1;
    small_cache[1] = 3;
    small_valid = 1;
    for (i = 2; i < SMALL_MAX; i++) {
        __int128 m = (__int128)(i - 1);
        __int128 coeff = 9 * m * m + 9 * m + 3;
        __int128 prev = small_cache[i - 1];
        __int128 prev2 = small_cache[i - 2];
        __int128 v = (coeff * prev - 27 * m * m * prev2) / ((__int128)i * i);
        small_cache[i] = v;
        if (v > cap || v < -cap) break;
        small_valid = i;
    }
    small_done = true;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (!small_done) fill_small();
        if (n <= (idx_t)small_valid) return from_i128(small_cache[n]);
        return 0; /* value does not fit in any fixed-width seq_t */
    }

    if (cache == NULL) {
        idx_t len = n + 1 < 2 ? 2 : n + 1;
        cache = alloc_zero(len);
        cache_len = len;
        cache[0] = 1;
        cache[1] = 3;
        max_computed = 1;
    } else if (n >= cache_len) {
        cache = grow_zero(cache, cache_len, n + 1);
        cache_len = n + 1;
    }

    for (idx_t i = max_computed + 1; i <= n; i++) {
        seq_t m = (seq_t)(i - 1);
        seq_t msq = m * m;
        seq_t coeff = 9 * msq + 9 * m + 3;
        seq_t isq = (seq_t)i * (seq_t)i;
        cache[i] = (coeff * cache[i - 1] - 27 * msq * cache[i - 2]) / isq;
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
