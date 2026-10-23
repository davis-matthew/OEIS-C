/*
 * A044317  Numbers n such that string 7,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains73
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044317: numbers n such that the base‑9 representation of n
 * contains the substring 73 but the base‑9 representation of n‑1 does not. */
/* The implementation searches for these numbers, caching results to
 * support calls in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains73.h"

/* Return the n-th term of the sequence, n ≥ 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* array of found terms */
    static size_t cap   = 0;       /* allocated capacity */
    static idx_t  len   = 0;       /* number of terms stored */
    static seq_t next_val = 1;     /* next candidate to test */

    /* Grow the cache until it contains the required term. */
    while (len < n)
    {
        bool has      = contains73(next_val);
        bool hasPrev  = contains73(next_val - (seq_t)1);

        if (has && !hasPrev)
        {
            /* Ensure space for another element. */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = next_val;
        }

        next_val = next_val + (seq_t)1;
    }

    return cache[n - 1];
}
