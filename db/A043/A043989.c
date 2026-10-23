/*
 * A043989  Numbers k such that 4 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_48or84
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043989: numbers k such that the digits 4 and 8 occur as
   adjacent digits (either 48 or 84) in the base‑9 expansion of k but not
   in that of k+1.  The implementation enumerates integers and stores
   the ones that satisfy the property. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_48or84.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next number to test */

    while (cache_len < (size_t)n) {
        /* test next_candidate */
        if (has_adjacent_48or84(next_candidate) &&
            !has_adjacent_48or84(next_candidate + 1)) {
            /* store it */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[cache_len - 1];
}
