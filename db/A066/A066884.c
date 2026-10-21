/*
 * A066884  Square array read by upward antidiagonals where the n-th row
 *          contains the positive integers with n binary 1's.
 * https://oeis.org/A066884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8001, big any n
 * Helpers:   comb_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066884: permutation of the positive integers.
   Each row r (r≥1) contains the integers with exactly r ones in binary,
   sorted increasingly.  The rows are read by upward antidiagonals:
   for the t-th diagonal (t≥1) we list the pairs (r,c) with r+c = t+1,
   decreasing r and increasing c.  This function returns the n‑th term
   (a(1)=1).  The computation uses the diagonal index d and a
   combinatorial unranking to find the c‑th integer with r ones. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/comb_le.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < n) ++d;  /* diagonal number d (0‑based) */

    idx_t prev_total = d * (d + 1) / 2;  /* terms before diagonal d */
    idx_t offset    = n - prev_total - 1;/* position within the diagonal, 0‑based*/
    idx_t r         = d + 1 - offset;    /* row index (number of ones)   */
    idx_t rank      = offset;            /* zero‑based rank within that row */

    seq_t val = 0;
    for (idx_t j = r; j >= 1; --j) {
        /* find the largest p such that C(p,j) ≤ rank */
        idx_t p = j - 1;
        while (comb_le(p + 1, j, rank) <= rank) ++p;

        /* set bit p if it fits in the current seq_t type */
        if (p < 8 * sizeof(seq_t)) val |= ((seq_t)1 << p);

        rank -= comb_le(p, j, rank);     /* update rank for next iteration */
    }
    return val;
}
