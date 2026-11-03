/*
 * A006999  Partitioning integers to avoid arithmetic progressions of length
 *          3.
 * https://oeis.org/A006999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*a(n-1)+2)/2) = a(n-1) + floor(a(n-1)/2) + 1,
 *            a(0) = 0
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 53, u64 n <= 108,
 *            u128 n <= 217, big any n
 * Validated: standard, pass, 55 terms through n = 980 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

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

    if (cache == NULL) {
        cache = grow_zero(NULL, 0, 1);
        cache[0] = 0;
        cache_len = 1;
    }

    if (n >= cache_len) {
        cache = grow_zero(cache, cache_len, n + 1);
        for (idx_t i = cache_len; i <= n; i++) {
            seq_t prev = cache[i - 1];
            cache[i] = prev + prev / 2 + 1;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
