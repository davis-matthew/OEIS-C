/*
 * A099896  A permutation of the natural numbers where a(n) = n XOR [n/2] XOR
 *          [n/4].
 * https://oeis.org/A099896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n XOR floor(n/2) XOR floor(n/4)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// a(1) = 1, a(2) = 3, a(3) = 2, a(4) = 7, ...
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast operands to the value type seq_t before XORing. */
    return ((seq_t)n) ^ ((seq_t)(n / 2)) ^ ((seq_t)(n / 4));
}
