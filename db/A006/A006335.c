/*
 * A006335  a(n) = 4^n*(3*n)!/((n+1)!*(2*n+1)!).
 * https://oeis.org/A006335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(3*n-1)*(3*n-2)*a(n-1)/((n+1)*(2*n+1)), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 698 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
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

    if (cache == NULL) {
        cache = alloc_zero(1);
        cache[0] = 1;
        cache_len = 1;
    }

    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache = grow_zero(cache, old_len, n + 1);
        cache_len = n + 1;

        for (idx_t i = old_len; i <= n; i++) {
            idx_t p = (idx_t)6 * (3 * i - 1) * (3 * i - 2);
            idx_t q = (i + 1) * (2 * i + 1);
            idx_t g = gcd_u64(p, q);
            p /= g;
            q /= g;
            /* q now divides a(i-1) exactly, since gcd(p, q) = 1 */
            seq_t t = cache[i - 1] / q;
            cache[i] = t * p;
        }
    }

    return cache[n];
}
