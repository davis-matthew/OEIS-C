/*
 * A047340  Numbers that are congruent to {0, 2, 3, 4} mod 7.
 * https://oeis.org/A047340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047340: numbers congruent to {0, 2, 3, 4} modulo 7.
   Formula: a(n) = 7 * floor((n-1)/4) + residue[(n-1) % 4]
   where residue = {0, 2, 3, 4}.  Indexing starts at 1. */

#include <stdint.h>

static const seq_t residues[4] = { (seq_t)0, (seq_t)2, (seq_t)3, (seq_t)4 };

seq_t a(idx_t n)
{
    // a(1) is the first term. No need to handle n == 0 for this sequence.
    idx_t idx = n - 1;              // 0‑based index
    idx_t t   = idx / 4;            // number of complete blocks of 4 allowed numbers
    idx_t r   = idx % 4;            // position within the current block

    return (seq_t)7 * (seq_t)t + residues[r];
}
