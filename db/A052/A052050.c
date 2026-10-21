/*
 * A052050  Squares whose digits occur with an equal minimal frequency of 2.
 * https://oeis.org/A052050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   square_has_minimal_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052050: Squares whose digits occur with an equal minimal frequency of 2.
   The code searches increasing squares and tests the property. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/square_has_minimal_two.h"

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Static state for incremental computation. */
    static seq_t *terms = NULL;          /* array of found terms */
    static size_t capacity = 0;          /* allocated capacity */
    static size_t length = 0;            /* number of terms stored */
    static idx_t last_k = 0;             /* last k processed (k^2) */

    /* Grow the terms array when needed. */
    if (capacity == 0) {
        capacity = 1024;
        terms = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }

    /* Generate squares until we have at least n terms. */
    while ((idx_t)length < n) {
        last_k += 1;
        seq_t sq = ((seq_t)last_k) * ((seq_t)last_k);
        if (square_has_minimal_two(sq)) {
            /* Append to the array. */
            if (length == capacity) {
                capacity *= 2;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * capacity);
            }
            terms[length++] = sq;
        }
    }

    return terms[n - 1];
}
