/*
 * A043173  Numbers k such that 3 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adj_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043173
 * a(n) = the n-th positive integer k such that the base‑8 representation
 * of k contains the adjacent digits 3 and 7 in either order, and the base-
 * 8 representation of k-1 does not.  Implementation uses incremental
 * search with a simple cache. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adj_pattern.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 1;          /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (has_adj_pattern(next_k) && !has_adj_pattern(next_k - 1)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
