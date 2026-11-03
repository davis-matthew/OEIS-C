/*
 * A031498  Numbers whose base-10 representation has 2 more 0's than 9's.
 * https://oeis.org/A031498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer whose decimal representation has exactly
   two more zeros than nines (OEIS A031498). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

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
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void ensure_cached(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        unsigned char cnt[10] = {0};
        digit_counts((seq_t)next_candidate, cnt);
        if (cnt[0] == cnt[9] + 2) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cached(n);
    return cache[n - 1];
}
