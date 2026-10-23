/*
 * A043154  Numbers k such that 0 and 3 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_0_3_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that base‑8 representation contains adjacent
   digits 0 and 3 (patterns 03 or 30) and the preceding integer k‑1
   does not contain such a pair. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_adjacent_0_3_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    /* dynamic cache of discovered terms; index 0 unused */
    static idx_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t found = 0;          /* number of terms cached */
    static idx_t last_checked = 0;   /* largest k examined */

    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        cache = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        cache_cap = new_cap;
    }

    if (n <= found)
        return (seq_t)cache[n];

    idx_t k;
    for (k = last_checked + 1; found < n; ++k) {
        if (has_adjacent_0_3_idx(k) && (!k || !has_adjacent_0_3_idx(k - 1))) {
            ++found;
            cache[found] = k;
        }
    }
    last_checked = k - 1;

    return (seq_t)cache[n];
}
