/*
 * A047427  Numbers that are congruent to {1, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 8 * floor((n-1)/5) + {1,3,4,5,6}[ (n-1) % 5 ]
 * Sequence A047427: numbers congruent to 1,3,4,5,6 mod 8 (offset 1)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute block index and position within block */
    idx_t k = (n - 1) / 5;          /* number of complete blocks of 5 good numbers */
    idx_t r = (n - 1) % 5;          /* position within the block [0..4] */

    /* Offsets of good numbers inside each 8‑number block */
    static const seq_t offsets[5] = { (seq_t)1,
                                      (seq_t)3,
                                      (seq_t)4,
                                      (seq_t)5,
                                      (seq_t)6 };

    /* Calculate the nth term */
    return ((seq_t)k * (seq_t)8) + offsets[r];
}
