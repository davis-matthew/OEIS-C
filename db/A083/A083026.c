/*
 * A083026  Numbers that are congruent to {0, 2, 4, 5, 7, 9, 11} mod 12.
 * https://oeis.org/A083026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * floor((n-1)/7) + {0,2,4,5,7,9,11}[(n-1) mod 7]
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=0, a(2)=2, a(3)=4, a(4)=5, a(5)=7, a(6)=9, a(7)=11 */

#include <stdint.h>  /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    /* Offsets for each position within a group of 7 (mod 12 residues). */
    static const idx_t offset[7] = {0, 2, 4, 5, 7, 9, 11};

    /* Number of complete groups of 7 before the n‑th term. */
    idx_t k = (n - 1) / 7;

    /* Position inside the current group (0 … 6). */
    idx_t r = (n - 1) % 7;

    /* Compute the value in a wide unsigned integer type then cast to seq_t. */
    idx_t value = 12 * k + offset[r];
    return (seq_t)value;
}
