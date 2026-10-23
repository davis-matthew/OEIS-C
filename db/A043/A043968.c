/*
 * A043968  Numbers k such that 1 and 2 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k such that the base‑9 representation
 * of k contains an adjacent pair of digits 1 and 2 (in either order),
 * while the base‑9 representation of k+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent12.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined for offset 1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t last_k = 0;

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    while (cache_len < n) {
        last_k++;
        if (has_adjacent12(last_k) && !has_adjacent12(last_k + (seq_t)1)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_k;
        }
    }

    return cache[n - 1];
}
