/*
 * A044010  Numbers k such that 1 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that “17” or “71” occur next to each other in the
   decimal expansion of k but not in the expansion of k+1.
   The function finds these numbers by scanning the integers in order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>   /* size_t */
#include "../utils/has_adjacent_17.h"

seq_t a(idx_t n)
{
    /* Lazy cache of discovered terms. */
    static seq_t *terms   = NULL;
    static size_t  terms_len = 0;   /* number of terms stored */
    static size_t  terms_cap = 0;   /* allocated capacity */
    static seq_t  scanned_k = 0;    /* next k to examine */

    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 */

    /* Grow cache until we have at least n terms. */
    while (terms_len < (size_t)n) {
        /* Ensure enough allocated space. */
        if (terms_len >= terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            seq_t *new_ptr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (new_ptr == NULL) { /* allocation failure – signal error */
                /* In an OEIS evaluation environment we can safely abort. */
                abort();
            }
            terms = new_ptr;
            terms_cap = new_cap;
        }

        /* Test whether the current k satisfies the condition. */
        if (has_adjacent_17(scanned_k) && !has_adjacent_17(scanned_k + (seq_t)1))
            terms[terms_len++] = scanned_k;

        scanned_k++;
    }

    return terms[(size_t)n - 1];
}
