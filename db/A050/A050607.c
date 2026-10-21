/*
 * A050607  Numbers k such that base 5 expansion matches
 *          (0|1|2)*((0|1)(3|4))?(0|1|2)*.
 * https://oeis.org/A050607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   qualifies_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050607: numbers s such that C(2s-1,s) is not divisible by 25.
   This is equivalent to the number of carries when adding s and (s-1) in
   base‑5 being at most 1 (Kummer's theorem).  The function generates the
   sequence by scanning integers, testing the carry property, and
   memoising the results. */

/* Include needed standard header */
#include <stdlib.h>
#include "../utils/qualifies_idx.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* static candidate to test next */
    static idx_t next_candidate = 0;

    /* Expand cache until it contains at least n terms */
    while (cache_size < n)
    {
        if (qualifies_idx(next_candidate))
        {
            /* ensure capacity */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure, return 0 */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
