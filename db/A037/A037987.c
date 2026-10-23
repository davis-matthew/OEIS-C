/*
 * A037987  Numbers whose maximal base-6 run length is 4.
 * https://oeis.org/A037987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   has_max_run4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037987: numbers whose maximal base-6 run length is 4.
   Sequence is computed by brute-force enumeration. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_max_run4.h"

/* Return the nth term (1‑based) of sequence A037987. */
seq_t a(idx_t n)
{
    /* Cache previously computed terms for fast repeated access. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  count = 0;
    static seq_t  next_candidate = (seq_t)1;

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Skip numbers that do not satisfy the property. */
        while (!has_max_run4(next_candidate))
            next_candidate++;

        /* Grow the cache if necessary. */
        if (count >= cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)          /* allocation failed */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        cache[count++] = next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
