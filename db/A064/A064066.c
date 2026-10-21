/*
 * A064066  n-th step is to add a(n) to each previous number a(k) (excluding
 *          itself, i.e., k < n) to produce n more terms of the sequence,
 *          starting with a(0)=1, a(1)=1.
 * https://oeis.org/A064066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064066: a(0)=a(1)=1; for n ≥ 1,
   generate terms a(n+1)..a(2n-1) with a(n+k)=a(n)+a(k) for 0 ≤ k < n. */

#include <stdlib.h>

static seq_t *seq = NULL;      /* array of computed terms */
static idx_t  seq_len = 0;     /* number of terms currently stored */
static size_t seq_cap = 0;     /* capacity of the array (in seq_t's) */
static idx_t  next_step = 1;   /* next index n to process (starting at 1) */

seq_t a(idx_t n)
{
    /* initialise storage on first call */
    if (seq_len == 0) {
        seq_cap = 4;                    /* arbitrary small start capacity */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;             /* out of memory, return zero */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)1;
        seq_len = 2;
        next_step = 1;
    }

    /* generate terms until we have a(n) */
    while (seq_len <= n) {
        /* if we accidentally run out of pre‑generated indices, the input pattern is wrong;
           but the problem guarantees only valid n. */
        seq_t a_next = seq[next_step];  /* a(next_step) */
        idx_t new_terms = next_step;    /* number of terms to add */

        idx_t new_len_needed = seq_len + new_terms;

        /* grow array if necessary */
        if (new_len_needed > seq_cap) {
            size_t new_cap = seq_cap;
            if (new_cap == 0) new_cap = 4;
            while (new_cap < (size_t)new_len_needed)
                new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* append new terms */
        for (idx_t k = 0; k < next_step; ++k)
            seq[seq_len + k] = a_next + seq[k];

        seq_len = new_len_needed;
        ++next_step;            /* move to the next index to process */
    }

    return seq[n];
}
