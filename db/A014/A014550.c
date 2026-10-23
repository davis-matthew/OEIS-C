/*
 * A014550  Binary reflected Gray code.
 * https://oeis.org/A014550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = decimal representation of the binary reflected Gray code
 *            of n The binary reflected Gray code of n is (n ^ (n>>1)).
 *            Converting its bits to decimal digits gives the terms listed in
 *            OEIS A014550
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute Gray code value */
    idx_t gray = n ^ (n >> 1);

    /* Handle n = 0 separately – its Gray code has one bit "0". */
    if (gray == 0)
        return 0;

    /* Determine number of bits in gray (most significant bit position + 1). */
    idx_t bitlen = 0;
    idx_t tmp = gray;
    while (tmp > 0) {
        tmp >>= 1;
        ++bitlen;
    }

    /* Convert each bit to a decimal digit, most significant first. */
    seq_t result = 0;
    for (idx_t i = bitlen; i > 0; --i) {
        seq_t bit = (gray >> (i - 1)) & 1;
        result = result * 10 + bit;
    }

    return result;
}
