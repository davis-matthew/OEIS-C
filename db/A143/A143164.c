/*
 * A143164  Numbers with digitsum 13, in increasing order.
 * https://oeis.org/A143164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9587 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose decimal digit sum equals 13
   using a simple brute‑force search with caching. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 safely */

    static seq_t *cache = 0;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    /* extend cache until we have n terms */
    while (cache_size < (size_t)n) {
        seq_t candidate = last_checked + (seq_t)1U;
        last_checked = candidate;

        if (digit_sum(candidate) == (seq_t)13U) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort(); /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
