/*
 * A219897  Numbers whose binary expansion has curling number > 1.
 * https://oeis.org/A219897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_curling_gt1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth positive integer whose binary expansion has a curling
   number > 1.  Sequence: OEIS A219897.  The curling number is >1
   iff there exists a block of bits that repeats immediately at the
   end of the binary representation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_curling_gt1.h"

seq_t a(idx_t n)
{
    /* Cache to avoid recomputing previously found terms. */
    static seq_t *cache = NULL;   /* cached terms      */
    static size_t cache_len = 0;  /* number of cached terms */
    static size_t cache_cap = 0;  /* allocated capacity    */
    static seq_t next_candidate = 3; /* next integer to test */

    if (n == 0) return 0; /* undefined for 0, but harmless */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Find the next integer with curling number > 1. */
        while (!has_curling_gt1(next_candidate)) {
            ++next_candidate;
        }
        /* Ensure capacity for one more term. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – abort execution. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
