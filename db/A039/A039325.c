/*
 * A039325  Numbers whose base-7 representation has the same nonzero number of
 *          3's and 6's.
 * https://oeis.org/A039325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_3_and_6_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039325: computed by brute‑force search for numbers whose
 * base‑7 representation contains an equal non‑zero number of 3's and 6's.
 * The first term corresponds to a(1) = 27. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_equal_3_and_6_nonzero.h"

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_num = 1;          /* next number to test */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        if (has_equal_3_and_6_nonzero(next_num)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                 /* Memory exhausted. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        next_num++;
    }

    return cache[n - 1];
}
