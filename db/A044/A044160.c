/*
 * A044160  Numbers n such that string 3,1 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044160: numbers n such that the base‑7 representation
   contains the substring "31" but n‑1 does not.
   The algorithm searches for the required numbers on demand,
   caching results so that calls may come in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has31.h"

/* Cache of computed terms.  term_cnt  : number of terms stored.
   term_cap  : capacity of the cache.
   last_checked : last integer examined during the search. */
static seq_t *terms = NULL;
static idx_t term_cnt = 0;
static idx_t term_cap = 0;
static seq_t last_checked = 1;

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (term_cnt < n) {
        /* Continue searching from the last integer examined. */
        last_checked++;

        /* Test the condition for the new candidate. */
        if (has31(last_checked) && !has31(last_checked - 1)) {
            /* Store the found term. */
            if (term_cnt >= term_cap) {
                /* Grow the cache: double its size. */
                idx_t new_cap = term_cap == 0 ? 8 : term_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms,
                                                    new_cap * sizeof(seq_t));
                terms = new_terms;
                term_cap = new_cap;
            }
            terms[term_cnt++] = last_checked;
        }
    }
    /* Retrieve the requested term. */
    return terms[n - 1];
}
