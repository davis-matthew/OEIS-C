/*
 * A047607  Numbers that are congruent to {0, 1, 2, 3, 5} mod 8.
 * https://oeis.org/A047607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,1,2,3,5}[(n-1) mod 5] (offset 1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t, not otherwise needed */

static const seq_t residues[5] = {0, 1, 2, 3, 5};

seq_t a(idx_t n)
{
    /* n >= 1; behavior for n = 0 is undefined (not needed). */
    idx_t q = (n - 1) / 5;          /* number of complete blocks of 8  */
    idx_t r = (n - 1) % 5;          /* index inside the 5 allowed residues */

    return (seq_t)(q * 8 + residues[r]);
}
