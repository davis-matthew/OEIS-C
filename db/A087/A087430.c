/*
 * A087430  Nonprimes n with gpf(n) > gpf(n+1), where gpf=A006530 (greatest
 *          prime factor).
 * https://oeis.org/A087430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonprimes n with gpf(n) > gpf(n+1)
   (OEIS A087430).  The function returns the n‑th term
   with an offset of 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* static cache of terms found so far */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static size_t terms_cnt  = 0;

    /* next candidate number to test */
    static idx_t next_candidate = 2;

    while (terms_cnt < (size_t)n)
    {
        idx_t cand = next_candidate++;

        if (cand <= 1)                 /* 0 and 1 are not counted */
            continue;

        /* gpf for cand */
        uint64_t gpf_c_ll = gpf((uint64_t)cand);
        if (gpf_c_ll == (uint64_t)cand)  /* skip primes */
            continue;

        /* gpf for cand+1 */
        uint64_t gpf_next_ll = gpf((uint64_t)(cand + 1));

        /* compare as seq_t */
        if ((seq_t)gpf_c_ll > (seq_t)gpf_next_ll)
        {
            /* ensure capacity */
            if (terms_cnt >= terms_cap)
            {
                size_t new_cap = terms_cap ? 2 * terms_cap : 4;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) abort();
                terms      = new_terms;
                terms_cap  = new_cap;
            }
            terms[terms_cnt++] = (seq_t)cand;
        }
    }
    return terms[n - 1];
}
