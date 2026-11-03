/*
 * A005705  Number of partitions of 4*n into powers of 4.
 * https://oeis.org/A005705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(floor(n/4)), a(0) = 1
 * Range:     u8 n <= 35, u16 n <= 242, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 879 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) {
        return cache[n];
    }

    idx_t new_len = n + 1;

    if (cache == NULL) {
        cache = alloc_zero(new_len);
        cache[0] = 1;
        cache_len = 1;
    } else {
        cache = grow_zero(cache, cache_len, new_len);
    }

    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = cache[i - 1] + cache[i / 4];
    }

    cache_len = new_len;
    return cache[n];
}
