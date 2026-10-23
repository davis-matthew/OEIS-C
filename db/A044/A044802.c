/*
 * A044802  Numbers k such that string 8,9 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that the decimal representation of k contains the
 *            substring "89" but the decimal representation of k+1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has89
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has89.h"

/* Main function definition */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored sequence terms */
    static size_t cache_size = 0;    /* number of stored terms */
    static size_t cache_cap  = 0;    /* allocated capacity */
    static seq_t next_candidate = 0; /* next number to test */

    /* If we already have enough terms, return the requested one. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        next_candidate++;                     /* test the next integer */

        /* Quick rejection: if next_candidate+1 contains "89" we skip. */
        if (has89(next_candidate) && !has89(next_candidate + 1)) {
            /* In the rare case the cache is full, grow it. */
            if (cache_cap <= cache_size) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) {                 /* allocation failed */
                    /* In a robust build you might handle this differently. */
                    exit(EXIT_FAILURE);
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
    }

    return cache[n - 1];
}
