/*
 * A044808  Numbers k such that string 9,5 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has95
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th integer k such that the decimal representation
   of k contains the substring "95" while the decimal representation of
   k+1 does not. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has95.h"

/* The primary function required by the exercise. */
seq_t a(idx_t n)
{
    /* Static structures for memoisation */
    static seq_t *cache = NULL;     /* array of discovered terms      */
    static size_t cache_size = 0;   /* number of terms stored         */
    static size_t cache_cap  = 0;   /* current capacity of the array  */
    static seq_t next_candidate = (seq_t)95; /* next integer to test          */

    /* Expand the cache until we have at least n terms */
    while (cache_size < n) {
        seq_t k = next_candidate;

        /* Check the required condition */
        if (has95(k) && !has95(k + (seq_t)1)) {
            /* Resize cache if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failed – aborting (cannot return a value) */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        ++next_candidate; /* move to the next integer */
    }

    /* Return the n‑th element (1‑based index). */
    return cache[n - 1];
}
