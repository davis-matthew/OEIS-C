/*
 * A047491  Numbers that are congruent to {4, 5, 7} mod 8.
 * https://oeis.org/A047491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + {4,5,7}[(n-1)%3]
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 4. For n==0 the result is undefined, return 0. */
    if (n == 0)
        return 0;

    /* Compute q = ⌊(n-1)/3⌋ and r = (n-1) mod 3. */
    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;

    /* Residues modulo 8 that belong to the sequence. */
    static const seq_t residues[3] = {4, 5, 7};

    /* Construct the result: 8*q + residue. */
    return (seq_t)8 * q + residues[r];
}
