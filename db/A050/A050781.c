/*
 * A050781  Numbers k such that sopfr(k) = sopfr(k - sopfr(k)).
 * https://oeis.org/A050781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   sopfr_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A050781: Numbers k such that sopfr(k) = sopfr(k - sopfr(k)).
 * This routine builds the sequence on demand by exhaustive search.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sopfr_u64.h"

/* ----- Dynamic array of found terms ---------------------------------- */
static seq_t *terms   = NULL;   /* stored terms  */
static idx_t term_cnt  = 0;      /* number of stored terms */
static idx_t term_cap  = 0;      /* current capacity         */
static uint64_t curr_k = 2;      /* next integer to test (skip trivial 1) */

static void push_term(seq_t v)
{
    if (term_cap == 0) {
        term_cap = 64;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    } else if (term_cnt >= term_cap) {
        term_cap <<= 1;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_cnt++] = v;
}

/* ----- Sequence entry point ------------------------------------------- */
seq_t a(idx_t n)
{
    while (term_cnt <= n) {
        seq_t sopk = sopfr_u64(curr_k);
        if (curr_k > (uint64_t)sopk) {              /* k - sopk > 0 required */
            uint64_t m = curr_k - (uint64_t)sopk;
            if (m > 0) {
                seq_t sopm = sopfr_u64(m);
                if (sopk == sopm) {
                    push_term((seq_t)curr_k);
                }
            }
        }
        ++curr_k;
    }
    return terms[n];
}
