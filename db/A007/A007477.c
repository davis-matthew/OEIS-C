/*
 * A007477  Shifts 2 places left when convolved with itself.
 * https://oeis.org/A007477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..n-2} a(k)*a(n-2-k), equivalently the D-finite
 *            recurrence (n+2)*a(n) = -(n+1)*a(n-1) + 4*(n-1)*a(n-2) +
 *            2*(4*n-9)*a(n-3) + 2*(2*n-7)*a(n-4), a(0) = 1, a(1) = 1, a(2) =
 *            1, a(3) = 2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 110,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_zero(cache_cap);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache_len = 4;
    }
    if (n < cache_len)
        return cache[n];

    if (n >= cache_cap) {
        idx_t nc = cache_cap;
        while (nc <= n)
            nc *= 2;
        cache = grow_zero(cache, cache_cap, nc);
        cache_cap = nc;
    }

    bool fixed = seq_is_fixed_width();
    for (idx_t i = cache_len; i <= n; i++) {
        if (fixed) {
            /* Convolution: exact modulo 2^w, no division needed */
            seq_t s = 0;
            for (idx_t k = 0; k + 2 <= i; k++)
                s += cache[k] * cache[i - 2 - k];
            cache[i] = s;
        } else {
            /* D-finite recurrence, exact division by (i+2) */
            seq_t s = cache[i - 2] * (4 * (i - 1))
                    + cache[i - 3] * (2 * (4 * i - 9))
                    + cache[i - 4] * (2 * (2 * i - 7));
            s -= cache[i - 1] * (i + 1);
            cache[i] = s / (i + 2);
        }
    }
    cache_len = n + 1;
    return cache[n];
}
