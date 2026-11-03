/*
 * A004211  Shifts one place left under 2nd-order binomial transform.
 * https://oeis.org/A004211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k)*Stirling2(n, k), via T(n,k) =
 *            2^(n-k)*Stirling2(n,k), T(n,k) = 2*k*T(n-1,k) + T(n-1,k-1),
 *            T(0,0) = 1, a(n) = Sum_k T(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 500 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *cache = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];

    idx_t newlen = n + 1;
    cache = grow_zero(cache, cache_len, newlen);

    seq_t *row = alloc_zero(n + 1);
    row[0] = 1;
    cache[0] = 1;

    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            row[k] = (seq_t)(2 * k) * row[k] + row[k - 1];
        }
        row[0] = 0;
        if (i >= cache_len) {
            seq_t s = 0;
            for (idx_t k = 1; k <= i; k++) s += row[k];
            cache[i] = s;
        }
    }

    release(row, n + 1);
    cache_len = newlen;
    return cache[n];
}
