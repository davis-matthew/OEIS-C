/*
 * A047534  Numbers that are congruent to {0, 1, 2, 3, 7} mod 8.
 * https://oeis.org/A047534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047534: numbers congruent to {0,1,2,3,7} mod 8, offset 1
   Formula: a(n) = 8*floor((n-1)/5) + [0,1,2,3,7][(n-1) mod 5]   (n ≥ 1) */

#include <stddef.h>

/* Return the n-th term of the sequence (a(1) = 0). */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to preserve precision. */
    seq_t nn = (seq_t)n;

    /* Compute the block number (k) and the position within the block (r). */
    seq_t k = (nn - (seq_t)1) / (seq_t)5;              /* floor((n-1)/5)   */
    seq_t r = (nn - (seq_t)1) % (seq_t)5;              /* (n-1) mod 5      */

    /* Residues allowed in each block of five numbers. */
    static const seq_t table[5] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)2,
        (seq_t)3,
        (seq_t)7
    };

    /* Compute the value: base=8*k plus the appropriate residue. */
    seq_t val = ((seq_t)8 * k) + table[(size_t)r];
    return val;
}
