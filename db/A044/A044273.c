/*
 * A044273  Numbers n such that string 2,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_substring_24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑9 representation of n
 * contains the substring "24" while the base‑9 representation of n‑1 does not.
 * The function builds the sequence on demand, caching previously found
 * terms for fast repeated queries.
 *
 * Formula: a(n) is the nth integer n≥1 with  9‑digit “24” and not in n−1
 * (definition only). The implementation is a straightforward search.
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_substring_24.h"

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined but harmless */

    /* Static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;
    static size_t cache_cap   = 0;

    /* Static variables used for incremental generation */
    static seq_t candidate   = 1; /* candidate number to test */
    static bool prev_has_24  = false; /* has_substring_24(candidate-1) */

    /* Grow cache if we haven't stored enough terms yet */
    while (cache_count < (size_t)n) {
        /* Allocate more space if necessary */
        if (cache_cap == 0 || cache_count >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr)
                abort(); /* memory failure */
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* Determine if the current candidate satisfies the condition */
        bool cur_has_24 = has_substring_24(candidate);
        if (cur_has_24 && !prev_has_24)
            cache[cache_count++] = candidate;

        /* Prepare for next iteration */
        prev_has_24 = cur_has_24;
        ++candidate;
    }

    return cache[(size_t)n - 1];
}
