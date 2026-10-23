/*
 * A102824  "True already", base 2, start 0: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 2 contains
 *          floor(a(n)/2) copies of the digit a(n) % 2, with a(0) = 0.
 * https://oeis.org/A102824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the least integer k such that * floor(k/2) equals the
 *            number of 0‑bits in the binary * concatenation of a(0), …,
 *            a(n‑1) when k is even, * or equals the number of 1‑bits when k
 *            is odd. * The sequence satisfies a(0)=0 and * a(n)=min( 2*cnt0 ,
 *            2*cnt1+1 ) after cnt0/1 have been * updated with...
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   add_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_bits.h"

seq_t a(idx_t n)
{
    static seq_t *seq_arr  = NULL;   /* sequence values */
    static idx_t  seq_len  = 0;      /* number of values computed */
    static idx_t  seq_cap  = 0;      /* allocated capacity */
    static idx_t  cnt0 = 0;          /* total 0‑bits seen so far */
    static idx_t  cnt1 = 0;          /* total 1‑bits seen so far */

    /* Initialise on first call or when seq_arr is NULL. */
    if (!seq_arr) {
        seq_cap = 16;
        seq_arr = (seq_t *)realloc(NULL, seq_cap * sizeof(seq_t));
        if (!seq_arr) return (seq_t)0;   /* Out of memory (unlikely). */

        seq_arr[0] = (seq_t)0;   /* a(0) */
        cnt0 = 1;                /* binary "0" has one zero bit */
        cnt1 = 0;
        seq_len = 1;
    }

    /* Extend the sequence up to index n. */
    for (idx_t i = seq_len; i <= n; ++i) {
        /* Ensure sufficient capacity. */
        if (seq_len >= seq_cap) {
            idx_t new_cap = seq_cap * 2;
            if (new_cap == 0) new_cap = 16;
            if (new_cap < seq_len + 1) new_cap = seq_len + 1;
            seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
            if (!seq_arr) return (seq_t)0;   /* Out of memory. */
        }

        /* Candidates for the next term. */
        seq_t cand_even = (seq_t)cnt0 * 2;          /* 2*cnt0  (even) */
        seq_t cand_odd  = (seq_t)cnt1 * 2 + 1;      /* 2*cnt1+1 (odd) */
        seq_t next = (cand_even <= cand_odd) ? cand_even : cand_odd;

        /* Store and update counts with bits of the new term. */
        seq_arr[seq_len++] = next;
        add_bits(next, &cnt0, &cnt1);
    }

    return seq_arr[n];
}
