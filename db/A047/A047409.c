/*
 * A047409  Numbers that are congruent to {0, 1, 4, 6} mod 8.
 * https://oeis.org/A047409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1)/4) + offset[(n-1)%4] where offset = {0,1,4,6}
 *            (a(1)=0, a(2)=1, a(3)=4, a(4)=6, a(5)=8, …)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n - 1;                    /* n >= 1 */
    seq_t block = (seq_t)m / 4;          /* number of full 8‑blocks */
    idx_t rem   = m % 4;                 /* position within the block */

    /* Offsets of the admissible residues within each block of 8 integers */
    static const seq_t offs[4] = {(seq_t)0, (seq_t)1, (seq_t)4, (seq_t)6};

    return block * 8 + offs[rem];
}
