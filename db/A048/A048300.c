/*
 * A048300  Numbers whose base-3 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_good_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A048300
 *  Numbers whose base-3 expansions, read from left to right,
 *  have run lengths that strictly increase (i.e. each run
 *  of identical digits is longer than the run immediately to its
 *  right).  This implementation enumerates the sequence from
 *  0 upward, memoising seen terms to support arbitrary call order.   */
 
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_good_sequence.h"
 

 
/* Sequence memoisation */
static seq_t *cache        = NULL;
static size_t cache_size   = 0;
static size_t cache_cap    = 0;
static seq_t  next_candidate = 0; /* next number to be tested */
 
seq_t a(idx_t n)
{
    /* Initialise the cache on first call */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* unlikely, but guard */
        cache[0] = 0;           /* a(0) = 0 */
        cache_size = 1;
        next_candidate = 1;     /* start testing from 1 */
    }
 
    /* If the requested term is already memoised, return it */
    if (n < cache_size)
        return cache[n];
 
    /* Generate terms until we reach the requested index */
    for (; cache_size <= n; ++next_candidate) {
        if (is_good_sequence(next_candidate)) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return 0;   /* guard against allocation failure */
            }
            cache[cache_size++] = next_candidate;
        }
    }
 
    return cache[n];
}
