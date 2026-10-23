/*
 * A044121  Numbers n such that string 3,1 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has31_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044121: a(n) is the nth positive integer whose base‑6
 * representation contains the substring 3,1 but the base‑6
 * representation of the previous integer does not.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has31_pattern.h"

/* Return the n‑th term of the sequence.  The function memoises all
   terms up to the largest index seen so far. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    /* The next integer to test for the pattern. */
    static seq_t next_candidate = (seq_t)1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t x = next_candidate;

        while (1) {
            /* Check the requirement for x and its predecessor. */
            if (has31_pattern(x) && !has31_pattern(x - (seq_t)1)) {
                /* Store the found term. */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;              /* double the capacity */
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = x;
                next_candidate = x + (seq_t)1;
                break;
            }
            ++x;
        }
    }

    return cache[n - (idx_t)1];
}
