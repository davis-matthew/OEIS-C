/*
 * A043129  Numbers k such that 3 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = kth positive integer k such that the base‑6
 *            representation of k contains the substring “35” or “53”, but the
 *            representation of k‑1 does not
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_35_or_53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_35_or_53.h"

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Grow the cache when needed. */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* External function required by the build. */
seq_t a(idx_t n) {
    if (n == 0)  /* Sequence is 1‑based; undefined for n = 0. */
        return (seq_t)0;

    /* Ensure the cache has at least n elements. */
    seq_t current = (cache_size == 0) ? (seq_t)1
                                      : cache[cache_size - 1] + 1;
    while (cache_size < n) {
        /* Search for the next qualifying number. */
        while (true) {
            if (has_35_or_53(current) && !has_35_or_53(current - 1)) {
                if (cache_size == cache_cap)
                    grow_cache();
                cache[cache_size++] = current;
                break;
            }
            current += (seq_t)1;
        }
        /* Move to the next candidate for the following search. */
        current += (seq_t)1;
    }
    return cache[n - 1];
}
