/*
 * A122683  Squares with strictly increasing digits.
 * https://oeis.org/A122683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122683 – squares with strictly increasing decimal digits.
   Computation method: generate every number whose decimal digits
   are strictly increasing (digits 1‑9 only), test each for being a
   perfect square, sort the matches, and cache the list. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* Lazy, static cache of all squares with strictly increasing digits. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Preallocate a modest capacity; we'll adjust if needed. */
        cache_cap = 512;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;

        /* 0 is a trivial square with increasing digits. */
        cache[cache_len++] = (seq_t)0;

        /* Generate all numbers with digits 1‑9 in strictly increasing order.
           Each such number corresponds to a non‑empty subset of {1,…,9}. */
        for (uint16_t mask = 1; mask < (1u << 9); ++mask) {
            seq_t num = (seq_t)0;
            for (int d = 0; d < 9; ++d) {
                if (mask & (1u << d))
                    num = num * (seq_t)10 + (seq_t)(d + 1);
            }
            seq_t root = floor_sqrt(num);
            if (root * root == num) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) { /* allocation failure: abort */
                        mem_free(cache); cache = NULL; return (seq_t)0;
                    }
                    cache = tmp;
                }
                cache[cache_len++] = num;
            }
        }

        /* Sort the list to obtain the sequence order. */
        mem_qsort(cache, cache_len, sizeof(seq_t), seq_cmp);

        initialized = true;
    }

    if (n == 0 || n > cache_len) {
        /* Outside the defined range – return 0. */
        return (seq_t)0;
    }
    return cache[n - 1];
}
