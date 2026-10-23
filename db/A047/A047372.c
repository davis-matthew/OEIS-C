/*
 * A047372  Numbers that are congruent to {1, 2, 3, 5} mod 7.
 * https://oeis.org/A047372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (n-1)/4 )*7 + [1,2,3,5][ (n-1)%4 ] (sequence of numbers
 *            congruent to 1,2,3,5 modulo 7)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset table for the four allowable residues mod 7. */
    static const unsigned char offset[4] = {1, 2, 3, 5};

    /* Compute quotient and remainder of (n-1) divided by 4. */
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* Convert to seq_t before multiplication to keep all intermediate
       operations in the target integer type. */
    return (seq_t)q * (seq_t)7 + (seq_t)offset[r];
}
