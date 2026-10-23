/*
 * A037971  Numbers whose maximal base-2 run length is 4.
 * https://oeis.org/A037971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   max_run_is_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Numbers whose maximal base-2 run length is 4.
   The sequence is defined as the set of positive integers whose binary
   representation contains a run of either four 0’s or four 1’s and no
   longer run.  The elements are listed in increasing order starting with
   a(1)=15.  
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/max_run_is_4.h"

/* Cache of previously found terms.  All static members persist between calls. */
static seq_t *terms          = NULL;   /* array of terms */
static size_t terms_cap      = 0;      /* current allocated capacity */
static size_t terms_count    = 0;      /* number of terms stored */
static uint64_t next_candidate = 1;     /* next integer to test */

static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();  /* OOM: terminate */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Main function: return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range, undefined behaviour not required */
    /* initialise cache on first use */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
    }

    while (terms_count < (size_t)n) {
        while (1) {
            seq_t cand = (seq_t)next_candidate;
            if (max_run_is_4(cand)) {
                /* store found term */
                ensure_terms_capacity(terms_count + 1);
                terms[terms_count++] = cand;
                break;  /* move to next candidate */
            }
            next_candidate++;
        }
        /* move past the number we just stored */
        next_candidate++;
    }

    return terms[n - 1];
}
