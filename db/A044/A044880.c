/*
 * A044880  In base 9, (sum of even run lengths)=(sum of odd run lengths).
 * https://oeis.org/A044880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   good_base9_run_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers whose base‑9 run‑length sums are equal. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_base9_run_property.h"

/* Main sequence function.  O(n) incremental generation with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but harmless */

    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t next_val    = 1;   /* next integer to test */

    while (cache_len < n) {
        /* Ensure enough capacity for a new term */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Find the next number that satisfies the property */
        while (1) {
            if (good_base9_run_property(next_val)) {
                cache[cache_len++] = next_val;
                next_val++;
                break;
            }
            next_val++;
        }
    }
    return cache[n - 1];
}
