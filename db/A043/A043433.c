/*
 * A043433  Numbers having one 3 in base 8.
 * https://oeis.org/A043433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_one_3_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043433: numbers having exactly one digit 3 in base‑8 representation */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_one_3_in_base8.h"

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* offset is 1; undefined below */

    /* Cache for previously computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = (seq_t)1;

    while (cache_len < n) {
        seq_t val = next_candidate;

        if (has_one_3_in_base8(val)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
