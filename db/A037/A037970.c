/*
 * A037970  Numbers whose maximal base-2 run length is 3.
 * https://oeis.org/A037970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_max_run3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose maximal consecutive run length
   in binary is exactly 3. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_max_run3.h"

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t next_candidate = 1;  /* next integer to test */

    /* Ensure that we can store at least n terms */
    while (cache_len < n) {
        /* Test whether the current candidate satisfies the condition */
        if (has_max_run3(next_candidate)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failure – terminate */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    /* n is 1‑based; indices in C are 0‑based */
    return cache[n - 1];
}
