/*
 * A167628  n^11 mod 13.
 * https://oeis.org/A167628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^11 mod 13
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = n % 13;
    seq_t result = 1;
    for (idx_t i = 0; i < 11; ++i)
        result = (result * (seq_t)rem) % 13;
    return result;
}
