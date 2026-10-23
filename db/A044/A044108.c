/*
 * A044108  Numbers k such that string 1,0 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1442, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044108: the n-th integer k such that the base‑6 representation of k contains
   the substring "10" while that of k‑1 does not. */
/* This implementation scans natural numbers once and memoises the results in a dynamic
   array, guaranteeing O(n) time overall for any sequence of queries. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has10.h"

/* Static storage for memoisation. */
static seq_t *terms    = NULL;      /* array of found terms */
static idx_t  terms_len   = 0;     /* number of stored terms */
static size_t terms_cap   = 0;     /* allocated capacity */
static idx_t  last_checked = 5;    /* largest k that has been examined */

seq_t a(idx_t n) {
    if (terms == NULL) {                /* initialise on first call */
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        last_checked = 5;
    }

    /* Extend the memoised list until we have n terms. */
    while (terms_len < n) {
        last_checked++;

        /* Determine whether last_checked satisfies the "10" criteria. */
        if (has10((seq_t)last_checked) &&
            !has10((seq_t)(last_checked - 1))) {

            /* Grow the array if necessary. */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
