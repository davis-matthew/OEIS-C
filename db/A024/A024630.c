/*
 * A024630  n written in fractional base 4/2.
 * https://oeis.org/A024630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2 * A007088(floor(n/2)) + (n mod 2) where A007088(k) is
 *            the decimal number whose digits are the binary representation of
 *            k (e.g., k = 13 -> A007088 = 1101)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   encode_binary_as_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/encode_binary_as_decimal.h"

seq_t a(idx_t n)
{
    seq_t b = encode_binary_as_decimal(n >> 1); /* floor(n/2) */
    return b * 2 + (n & 1);
}
