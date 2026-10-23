/*
 * A044011  Numbers k such that 1 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has18or81
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044011: numbers k such that the decimal representation of k contains
   the substring "18" or "81" but the decimal representation of k+1 does not.
   Computed by incremental search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has18or81.h"

/* Return the nth element (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t found = 0; /* how many terms are already stored */
    static idx_t last_checked = 1; /* last number examined */

    /* Ensure the cache can hold at least n terms. */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (found < n) {
        seq_t k = (seq_t)last_checked;
        if (has18or81(k) && !has18or81(k + 1)) {
            cache[found++] = k;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
