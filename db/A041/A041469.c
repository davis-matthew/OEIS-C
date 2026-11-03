/*
 * A041469  Denominators of continued fraction convergents to sqrt(250).
 * https://oeis.org/A041469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 8886*a(n-7) + a(n-14) for n > 13 Initial values: a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=16, a(4)=53, a(5)=228, a(6)=281, a(7)=8658,
 *            a(8)=8939, a(9)=44414, a(10)=142181, a(11)=470957,
 *            a(12)=2026009, a(13)=2496966
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 183 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static const seq_t init_vals[14] = {
    1, 1, 5, 16, 53, 228, 281,
    8658, 8939, 44414, 142181, 470957, 2026009, 2496966
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len  = 0;

seq_t a(idx_t n) {
    /* Handle cached values first */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure we have enough space to store up to n */
    ensure_cache((idx_t)(n + 1), &cache, &cache_cap);

    /* Compute terms from current length up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 14) {
            cache[i] = init_vals[i];
        } else {
            /* Use 128‑bit arithmetic to avoid intermediate overflow
               when seq_t is 64‑bit; for larger seq_t the cast is
               harmless. */
            seq_t val = (seq_t)(((__uint128_t)8886) * cache[i - 7] + cache[i - 14]);
            cache[i] = val;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
