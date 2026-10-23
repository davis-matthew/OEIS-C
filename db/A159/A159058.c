/*
 * A159058  A102370(n) modulo 8 .
 * https://oeis.org/A159058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   a(n) = ((b1 ^ b2) << 2) | ((b0 ^ b1) << 1) | b0 where b0 = n &
 *            1, b1 = (n >> 1) & 1, b2 = (n >> 2) & 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract the lowest three bits of n */
    uint8_t b0 = (uint8_t)(n & 1);
    uint8_t b1 = (uint8_t)((n >> 1) & 1);
    uint8_t b2 = (uint8_t)((n >> 2) & 1);

    /* Compute the bits of the output */
    uint8_t c2 = b1 ^ b2;
    uint8_t c1 = b0 ^ b1;
    uint8_t c0 = b0;

    /* Construct the result as a seq_t value */
    return (seq_t)((c2 << 2) | (c1 << 1) | c0);
}
