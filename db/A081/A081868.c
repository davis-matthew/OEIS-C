/*
 * A081868  Numbers k such that Sum_{j=1..k} (binomial(2*j,j) mod 3) is even.
 * https://oeis.org/A081868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081868:  a(n) = the n‑th integer k such that the sum of
     C(2*j,j) mod 3 for 1 ≤ j ≤ k is even.
   The code computes this by testing each integer j, counting
   terms for which binomial(2*j,j) ≡ 1 (mod 3).
   C(2*j,j) ≡ 1 (mod 3)  iff  j has no digit 2 in base‑3 and the
   number of digits 1 is even.  We keep a running parity of the
   count of such “good” j’s and record a position k whenever that
   parity is even. */

/* required headers for standard integer and memory functions */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/good_idx.h"

/* Main sequence function.  Static state allows calls in any order. */
seq_t a(idx_t n)
{
    /* static storage for the sequence values */
    static seq_t *seq = NULL;           /* array of a(1), a(2), … */
    static size_t seq_len = 0;          /* number of stored terms */
    static size_t seq_cap = 0;          /* allocated capacity */
    static idx_t cur_j = 0;             /* last j processed so far */
    static unsigned char parity = 0;    /* parity of count of good j’s */

    /* Grow the array if needed */
    if (seq_cap == 0) {
        /* start with a small capacity */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n values */
    while (seq_len < (size_t)n) {
        ++cur_j;
        if (good_idx(cur_j))
            parity ^= 1;                /* flip parity on a good j */

        if (parity == 0) {
            /* store the current j as a new term */
            if (seq_len == seq_cap) {
                /* double capacity */
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cur_j;
        }
    }

    return seq[n - 1];
}
