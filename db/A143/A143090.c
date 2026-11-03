/*
 * A143090  Aliquot sequence starting at 12.
 * https://oeis.org/A143090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=12, a(n+1)=sigma_of(a(n)) - a(n)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Validated: partial, pass, 104 terms through n = 103 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
static idx_t cache_len = 0;

static void ensure_cache(idx_t needed) {
    if (cache_len >= needed) return;
    if (cache == NULL) {
        cache = seq_alloc(needed);
    } else {
        cache = seq_realloc(cache, cache_len, needed);
    }
    cache_len = needed;
}

seq_t a(idx_t n) {
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)12;
        cache_len = 1;
    }

    if (n < cache_len) return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;
        if ((idx_t)prev <= 1) {
            next = 0;
        } else {
            idx_t prev_small = (idx_t)prev;
            next = (seq_t)(sigma_of(prev_small) - prev_small);
        }
        ensure_cache(i + 1);
        cache[i] = next;
    }
    cache_len = n + 1;
    return cache[n];
}
