/*
 * A044040  Numbers k such that 8 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_8_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044040: numbers k such that the decimal representation of k
   contains the substring "89" or "98" while k+1 does not. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_8_9.h"

/* Return the n-th element of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;     /* number of cached terms */
    static size_t cached_cap  = 0;     /* allocated capacity of cache */
    static seq_t next_candidate = 1;   /* next number to test */

    /* Ensure cache large enough for the requested index */
    while (cached_size < (size_t)n) {
        /* Scan forward until we find a valid term. */
        while (true) {
            if (has_adjacent_8_9(next_candidate) &&
                !has_adjacent_8_9(next_candidate + 1)) {
                /* Append to cache */
                if (cached_size == cached_cap) {
                    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
                    seq_t *new_cache =
                        (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) /* allocation failure – exit */
                        exit(EXIT_FAILURE);
                    cache = new_cache;
                    cached_cap = new_cap;
                }
                cache[cached_size++] = next_candidate;
                break;
            }
            ++next_candidate;
        }
        ++next_candidate; /* move past the found term for next search */
    }

    return cache[(size_t)(n - 1)];
}
