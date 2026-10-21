/*
 * A074782  Let b(1)=b(2)=1, b(k) = (2^b(k-1)+2^b(k-2)) (mod k); sequence
 *          gives values of n such that b(n)=0.
 * https://oeis.org/A074782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mod_pow2_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A074782: a(n) returns the n‑th integer k for which
   b(k) = 0.  Here b is defined by the recurrence
   b(1) = b(2) = 1,
   b(k) = (2^b(k-1) + 2^b(k-2)) mod k.
   The function iterates the recurrence, collects the k where b(k)=0,
   and returns the n‑th such value.  All arithmetic is performed in
   the seq_t type; indices and exponents that are guaranteed to fit
   in uint64_t are kept as idx_t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow2_exp.h"

/* Global state for memoization (keeps the sequence state between calls) */
static seq_t prev2 = (seq_t)1;   /* b(1) */
static seq_t prev1 = (seq_t)1;   /* b(2) */
static idx_t  cur_k = (idx_t)2;  /* last computed k */

/* Array holding the indices k for which b(k) == 0. */
static idx_t *seq_vals = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;

/* Return the n-th value of A074782 (1‑based indexing). */
seq_t a(idx_t n)
{
    while (seq_len < (size_t)n)
    {
        /* Compute next b(k) for k = cur_k+1 */
        idx_t   k    = cur_k + (idx_t)1;
        idx_t   exp1 = (idx_t)prev1;   /* b(k-1)  */
        idx_t   exp2 = (idx_t)prev2;   /* b(k-2)  */

        seq_t pow1 = mod_pow2_exp(exp1, (seq_t)k);
        seq_t pow2 = mod_pow2_exp(exp2, (seq_t)k);
        seq_t val  = (pow1 + pow2) % (seq_t)k;   /* b(k) */

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = val;
        cur_k = k;

        /* Store k if it is a zero of b. */
        if (val == (seq_t)0)
        {
            if (seq_len == seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(seq_vals, new_cap * sizeof(idx_t));
                seq_vals = tmp;
                seq_cap  = new_cap;
            }
            seq_vals[seq_len++] = k;
        }
    }

    return (seq_t)seq_vals[n - 1];
}
