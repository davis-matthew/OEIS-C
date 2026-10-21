/*
 * A065609  Positive m such that when written in binary, no rotated value of m
 *          is greater than m.
 * https://oeis.org/A065609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 8907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: brute‑force test of each integer for the
   maximal rotation property (a(n) is the n‑th smallest positive
   integer whose binary rotations are all ≤ the integer itself). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"   /* provides bitlen_seq_t */

#define SEQ_T_BITS (sizeof(seq_t) * 8)

/* --------------------------------------------------------------------- */
/* Test whether m is the maximum among all its cyclic right rotations. */
static bool is_max_rotation(seq_t m)
{
    unsigned L = bitlen_seq_t(m);        /* number of bits in m (L ≥ 1) */
    if (L == 1)
        return true;                    /* single–bit numbers are always maximal */

    /* Build a mask with the lowest L bits set to 1. */
    seq_t mask;
    if (L == SEQ_T_BITS)
        mask = ~(seq_t)0;
    else
        mask = ((seq_t)1 << L) - 1;

    for (unsigned k = 1; k < L; ++k) {
        seq_t rot = ((m >> k) | (m << (L - k))) & mask;
        if (rot > m)
            return false;               /* found a larger rotation */
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Dynamic storage of the sequence values. */
static seq_t *terms = NULL;        /* array of generated terms */
static size_t terms_size = 0;      /* how many terms we have */
static size_t terms_cap  = 0;      /* allocated capacity of the array */
static seq_t next_candidate = (seq_t)1;  /* next integer to test */

/* --------------------------------------------------------------------- */
/* Public entry point: returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Safety: ignore n == 0, but the function is not expected to be called with it. */
    if (n == 0)
        return (seq_t)0;

    /* Expand the term list until we have at least n terms. */
    while (terms_size < (size_t)n) {
        if (is_max_rotation(next_candidate)) {
            /* Ensure the array has enough space. */
            if (terms_size == terms_cap) {
                size_t newcap = (terms_cap == 0) ? 16 : terms_cap * 2;
                seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!newptr) {
                    /* Allocation failure – abort (safe for the exercise). */
                    abort();
                }
                terms = newptr;
                terms_cap = newcap;
            }
            terms[terms_size++] = next_candidate;
        }
        ++next_candidate;   /* advance to the next integer to test */
    }

    return terms[(size_t)(n - 1)];
}
