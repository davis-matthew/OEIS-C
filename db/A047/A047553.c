/*
 * A047553  Numbers that are congruent to {0, 2, 6, 7} mod 8.
 * https://oeis.org/A047553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/4)*8 + offset[(n-1)%4] with offset = {0,2,6,7}
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by the sequence offset; behaviour for n == 0 is undefined */
    idx_t m = n - 1;               /* zero‑based index */
    idx_t block = m / 4;           /* number of complete 8‑blocks before a(n) */
    idx_t rem   = m % 4;           /* position inside the current block */

    /* offsets of the allowable residues in each 8‑block: 0, 2, 6, 7 */
    static const seq_t offset[4] = { (seq_t)0, (seq_t)2, (seq_t)6, (seq_t)7 };

    seq_t val = (seq_t)block * (seq_t)8 + offset[rem];
    return val;
}
