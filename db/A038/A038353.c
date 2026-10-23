/*
 * A038353  Numbers whose base-6 representation has the same nonzero number of
 *          2's and 4's.
 * https://oeis.org/A038353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_equal_nonzero_2_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038353: Numbers whose base‑6 representation has the same
 * non‑zero number of digits 2 and 4. The function generates terms by
 * a linear search with memoization. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_nonzero_2_4.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t current = 0;

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        current++;                      /* next integer to test   */
        if (has_equal_nonzero_2_4(current)) {
            /* Ensure capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();    /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current;
        }
    }
    return cache[n - 1];
}
