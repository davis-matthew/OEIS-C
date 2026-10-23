/*
 * A039404  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 4's.
 * https://oeis.org/A039404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 7957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_1_and_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having equal non‑zero counts of 1 and 4 in decimal representation */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_1_and_4.h"

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset, harmless */

    static seq_t *cache      = NULL;
    static idx_t   cache_cap = 0;
    static idx_t   cache_len = 0;
    static seq_t   last_number = 0;

    if (cache_len < n) {
        /* Grow capacity if necessary. */
        idx_t needed_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (needed_cap < n) needed_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure -> short‑circuit */
        cache = tmp;
        cache_cap = needed_cap;

        /* Generate terms until we have at least n entries. */
        while (cache_len < n) {
            ++last_number;
            if (has_equal_1_and_4(last_number))
                cache[cache_len++] = last_number;
        }
    }
    return cache[n - 1];
}
