/*
 * A047301  Numbers that are congruent to {0, 2, 3, 4, 6} mod 7.
 * https://oeis.org/A047301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*floor((n-1)/5) + [0,2,3,4,6][ (n-1) mod 5 ]
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 6, a(6) = 7, ... */

#include <stddef.h>

seq_t
a(idx_t n)
{
    /* Compute the block index and the offset within the block. */
    idx_t m = n - 1;                 /* n >= 1 guarantees m >= 0          */
    idx_t block = m / 5;
    idx_t pos   = m % 5;

    /* Offsets of the allowed residues within each 7‑number block. */
    static const seq_t offsets[5] = {0, 2, 3, 4, 6};

    /* Compute the value using seq_t arithmetic. */
    seq_t base = (seq_t)block * ((seq_t)7);
    return base + offsets[pos];
}
