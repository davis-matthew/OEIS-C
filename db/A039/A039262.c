/*
 * A039262  Numbers whose base-12 representation has the same number of 6's
 *          and 9's.
 * https://oeis.org/A039262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   good_sequence_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039262: numbers whose base‑12 representation has the
   same number of 6's and 9's.  
   Computed by generating integers in ascending order and testing the
   digit property. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_sequence_number.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;

    /* Extend cache until we have at least n entries */
    while (cache_len < n) {
        if (good_sequence_number(next_candidate)) {
            /* Ensure space for a new value */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 32;
                if (cache_len + 1 > new_cap) new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
