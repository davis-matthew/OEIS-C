/*
 * A039186  Numbers whose base-11 representation has the same number of 3's
 *          and 8's.
 * https://oeis.org/A039186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑11 representation
 *            contains the same number of digit 3 and digit 8
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal3_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal3_8.h"

seq_t a(idx_t n)
{
    /* Lazy memoization of the sequence. */
    static seq_t *cache = NULL;          /* Stored valid terms */
    static idx_t cache_cnt = 0;          /* Number of terms stored */
    static size_t cache_cap = 0;         /* Current capacity */
    static seq_t current = 0;            /* Next integer to test */

    /* Allocate initial cache if needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* Allocation failed; return 0 as fallback. */
        cache_cnt = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < n) {
        if (equal3_8(current)) {
            /* Store the hit.  Grow cache if necessary. */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0; /* Allocation failed. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = current;
        }
        ++current; /* Next integer to test */
    }

    return cache[n - 1];
}
