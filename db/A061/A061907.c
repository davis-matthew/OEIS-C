/*
 * A061907  The iterative cycle: n -> sum of digits of n^2 has only four
 *          distinct elements.
 * https://oeis.org/A061907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   f_of, orbit_has_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the n‑th positive integer whose
   iterated map f(x) = sum of decimal digits of x² has an
   eventual cycle containing exactly four distinct values. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/f_of.h"
#include "../utils/orbit_has_four.h"

/* Cache of previously found terms. */
static seq_t *terms    = NULL;
static size_t terms_len = 0;   /* number of stored terms   */
static size_t terms_cap = 0;   /* allocated capacity       */

/* Next integer to test when searching for terms. */
static seq_t next_candidate = 1;

/* Grow the allocated array when it becomes full. */
static void grow_terms(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!ptr) abort();            /* out of memory */
        terms = ptr;
        terms_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for 0, return harmless sentinel */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        if (orbit_has_four(next_candidate)) {
            grow_terms();
            terms[terms_len++] = next_candidate;
        }
        ++next_candidate;   /* try next integer on next iteration */
    }

    return terms[n - 1];
}
