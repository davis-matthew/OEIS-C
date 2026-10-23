/*
 * A047504  Numbers that are congruent to {1, 2, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [1,2,3,4,5,7][(n-1) mod 6]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are congruent to {1,2,3,4,5,7} modulo 8.
   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=9, ... */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Offsets within each block of 6 valid numbers. */
    static const seq_t offsets[6] = {1, 2, 3, 4, 5, 7};

    /* Compute block index and position within the block.  */
    idx_t m = n - 1;                /* zero‑based index */
    idx_t block = m / 6;            /* number of complete blocks before */
    idx_t pos   = m % 6;            /* position inside the current block */

    /* Base value for the current block */
    seq_t base = (seq_t)8 * (seq_t)block;

    return base + offsets[(size_t)pos];
}
