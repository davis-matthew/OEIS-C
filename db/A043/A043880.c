/*
 * A043880  Numbers k such that 0 and 1 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_adjacent01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Generate numbers k where the base-4 representation contains
   the adjacency of digits 0 and 1 (in either order) and the base-4
   representation of k+1 does NOT contain such adjacency.
   a(1) = 4 is the first such number. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent01.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t next_k = 0;          /* next k to test */

    while (cache_len < n) {
        seq_t k = next_k;
        if (has_adjacent01(k)) {
            seq_t k1 = k + 1;
            if (!has_adjacent01(k1)) {
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
        next_k++;
    }
    return cache[n - 1];
}
