/*
 * A037995  Numbers whose maximal base-8 run length is 4.
 * https://oeis.org/A037995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_run_length4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose maximal base‑8 run length is exactly 4 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_run_length4.h"

/* Dynamically cached array of terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_candidate = 1;

/* Return the n‑th term of the sequence.   (n >= 1) */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        /* Find the next integer satisfying the property. */
        while (!has_run_length4(next_candidate))
            ++next_candidate;

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next_candidate;
        ++next_candidate;   /* next test will start here */
    }

    return cache[n - 1];
}
