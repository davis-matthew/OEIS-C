/*
 * A047495  Numbers that are congruent to {2, 4, 5, 7} mod 8.
 * https://oeis.org/A047495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {2,4,5,7}[(n-1)%4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, return 0 */

    /* block index and position within block */
    idx_t m = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* offsets for residues 2,4,5,7 modulo 8 */
    static const seq_t offsets[4] = {2, 4, 5, 7};

    /* compute a(n) */
    return (seq_t)8 * (seq_t)m + offsets[r];
}
