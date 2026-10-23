/*
 * A044425  Numbers n such that string 9,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_93
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that decimal string "93" occurs in n but not in n-1.
   Implementation uses an incremental search from 93, storing discovered terms
   in a static cache so that calls with arbitrary indexes are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains_93.h"

seq_t a(idx_t n)
{
    /* Static cache to store computed sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Next number to test during incremental generation. */
    static seq_t next_candidate = 93; /* first term of the sequence */

    if (n == 0) return 0; /* undefined for offset 1, but safe default */

    /* Grow cache if necessary. */
    if (cache_len < (size_t)n) {
        size_t required = (size_t)n;
        if (cache_cap == 0) {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap < required) {
            cache_cap = required > 2 * cache_cap ? required : 2 * cache_cap;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Extend cache by scanning numbers until we have enough terms. */
        while (cache_len < required) {
            if (contains_93(next_candidate)) {
                seq_t prev = next_candidate - 1;
                if (!contains_93(prev))
                    cache[cache_len++] = next_candidate;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
