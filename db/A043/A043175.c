/*
 * A043175  Numbers k such that 4 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_4_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that 4 and 6 occur juxtaposed in base-8 representation of k but not of k-1.
   Computed by exhaustive search. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_4_6.h"

/* Return the n‑th such number (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; this value is unused */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t last_checked = 0;

    while (cache_size < (size_t)n) {
        seq_t k = last_checked + 1;
        if (has_adjacent_4_6(k) && !has_adjacent_4_6(k - 1)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        last_checked = k;
    }

    return cache[n - 1];
}
