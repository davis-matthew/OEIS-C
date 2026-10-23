/*
 * A044219  Numbers n such that string 4,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_40_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044219: Numbers n such that the base‑8 representation of n
 * contains the substring "40" but the base‑8 representation of n-1 does not.
 * This implementation uses a simple brute‑force search with memoisation.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_40_in_base8.h"

/* Main function.  The sequence is generated on demand and stored in a
 * static array so that repeated calls with any order of 'n' are fast. */
seq_t a(idx_t n)
{
    /* Static state across calls. */
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of cached terms */
    static idx_t next_test = 1;   /* next integer to test */

    if (n == 0) { /* offset is 1; undefined behaviour for n<1 is allowed */
        return (seq_t)0;
    }

    /* Grow cache if it does not yet contain n terms. */
    if (cache_len < n) {
        /* Ensure sufficient capacity. */
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Keep searching until we have n terms. */
        while (cache_len < n) {
            /* Find next integer satisfying the condition. */
            for (;;) {
                seq_t cur   = (seq_t)next_test;
                seq_t prev  = (seq_t)(next_test - 1);
                if (has_40_in_base8(cur) && !has_40_in_base8(prev))
                    break;
                ++next_test;
            }
            cache[cache_len++] = (seq_t)next_test;
            ++next_test; /* increment for the next search */
        }
    }
    return cache[n - 1];
}
