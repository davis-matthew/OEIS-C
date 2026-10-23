/*
 * A047519  Numbers that are congruent to {1, 2, 3, 4, 6, 7} mod 8.
 * https://oeis.org/A047519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* a(n) = 8 * floor((n-1)/6) + offset[(n-1)%6]
 * offset{0..5} = {1,2,3,4,6,7}  (the allowed residues mod 8)
 */
seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 6;
    idx_t r = (n - 1) % 6;
    static const seq_t offset[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                     (seq_t)4, (seq_t)6, (seq_t)7 };
    return (seq_t)q * (seq_t)8 + offset[(size_t)r];
}
