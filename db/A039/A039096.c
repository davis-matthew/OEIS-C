/*
 * A039096  Numbers whose base-9 representation has the same number of 2's and
 *          8's.
 * https://oeis.org/A039096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_2_and_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑9 representation has an equal number of 2's and 8's.
   The function generates the sequence lazily and keeps the values in a
   cache so that calls in any order are fast.  The implementation is
   purely integer based; no floating‑point or external helper functions
   are used. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_2_and_8.h"

/* Return the n-th element of A039096 (offset 1).  n >= 1. */
seq_t a(idx_t n)
{
    /* cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache       = NULL;
    static size_t cache_size  = 0;   /* number of entries currently in cache */
    static size_t cache_cap   = 0;   /* allocated capacity (in entries) */

    if (n < (idx_t)1)                /* invalid index – return 0 */
        return (seq_t)0;

    /* Grow the cache until it contains at least n entries. */
    if (cache_size < (size_t)n) {
        /* Initial allocation if the cache is empty. */
        if (cache_size == 0) {
            cache_cap  = 16;
            cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure */
            cache[0] = (seq_t)0;
            cache_size = 1;
        }

        /* Start searching from the next integer after the last cached one. */
        seq_t current = cache[cache_size - 1] + (seq_t)1;

        while (cache_size < (size_t)n) {
            /* Find the next candidate with equal 2's and 8's. */
            seq_t candidate = current;
            while (!has_equal_2_and_8(candidate)) {
                ++candidate;
            }

            /* Ensure we have space for one more entry. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                     /* allocation failure */
                    freel: /* nothing to free; we simply leak to return 0 */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Append the found number. */
            cache[cache_size] = candidate;
            ++cache_size;
            current = candidate + (seq_t)1;
        }
    }

    return cache[n - 1];
}
