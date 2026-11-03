/*
 * A028164  Expansion of 1/((1-4x)(1-10x)(1-11x)(1-12x)).
 * https://oeis.org/A028164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 37; for n ≥ 2, a(n) = 23*a(n-1) - 132*a(n-2) +
 *            (10^(n+1) - 4^(n+1))/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 37;

    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = seq_alloc(cache_cap);
        cache[0] = 1;
        cache[1] = 37;
        cache_len = 2;
    }

    if (n >= cache_len) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            seq_t *new_cache = seq_realloc(cache, cache_cap, needed);
            cache = new_cache;
            cache_cap = needed;
        }

        seq_t pow10_cur = ipow_u64_idx(10, cache_len);
        seq_t pow4_cur  = ipow_u64_idx(4, cache_len);

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term  = 23 * cache[i-1] - 132 * cache[i-2];
            seq_t pow10_next = pow10_cur * (seq_t)10;
            seq_t pow4_next  = pow4_cur  * (seq_t)4;
            term += (pow10_next - pow4_next) / 6;
            cache[i] = term;
            pow10_cur = pow10_next;
            pow4_cur  = pow4_next;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
