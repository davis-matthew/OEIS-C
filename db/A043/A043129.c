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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if a base‑6 representation of n contains the
   consecutive digits 3 and 5 or 5 and 3. */
static bool has_35_or_53(seq_t n) {
    bool has_prev = false;
    seq_t prev_digit = 0;
    while (n > 0) {
        seq_t d = n % 6;
        if (has_prev && ((prev_digit == 3 && d == 5) || (prev_digit == 5 && d == 3)))
            return true;
        prev_digit = d;
        has_prev = true;
        n /= 6;
    }
    return false;
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Grow the cache when needed. */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
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
