/*
 * A043764  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 1 mod 10.
 * https://oeis.org/A043764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose binary representation *
 *            has a number of runs congruent to 1 modulo 10. * * A run is a
 *            maximal sequence of identical bits; e.g. 101011 has 5 runs. *
 *            This implementation finds the term by simple trial generation
 *            and * caches previously found terms for fast repe...
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   run_count_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/run_count_seq.h"

/* Cached terms of the sequence.
 * terms[0] = a(1), terms[1] = a(2), etc.
 */
static seq_t *terms   = NULL;
static idx_t  tcount  = 0;     /* number of cached terms */
static idx_t  tcap    = 0;     /* capacity of the terms array */
static idx_t  last_cand = 0;   /* the largest candidate examined so far */

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Grow the cached array if necessary. */
    if (terms == NULL) {
        tcap = 16;
        terms = (seq_t *)malloc(tcap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms cached. */
    idx_t cand = last_cand + 1;
    while (tcount < n) {
        seq_t x = (seq_t)cand;
        int runs = run_count_seq(x);
        if (runs % 10 == 1) {
            if (tcount == tcap) {
                tcap = tcap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, tcap * sizeof(seq_t));
                if (!new_terms) {
                    /* Out of memory – terminate the program. */
                    abort();
                }
                terms = new_terms;
            }
            terms[tcount++] = x;
        }
        ++cand;
    }

    last_cand = cand - 1;
    return terms[n - 1];
}
