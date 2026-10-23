/*
 * A044197  Numbers n such that string 1,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that base‑8 representation of k contains
 *            "12" but not that of k‑1. Implementation: brute‑force scan with
 *            memoisation
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has12_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has12_base8.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 1; /* next number to test */

    /* Grow the cache until we have at least n terms */
    while (cache_size < n) {
        /* Ensure there is room for another term */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)               /* allocation failed */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t cur = (seq_t)next_candidate;
        bool cur_has = has12_base8(cur);
        bool prev_has = has12_base8((seq_t)(next_candidate - 1));
        if (cur_has && !prev_has)
            cache[cache_size++] = cur;

        ++next_candidate;
    }

    return cache[n - 1];
}
