/*
 * A091064  In binary representation: length of longest proper prefix of n,
 *          that is also a suffix.
 * https://oeis.org/A091064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the length of the longest proper prefix of the binary
 *            representation of n that is also a suffix (i.e., the longest
 *            border of the binary string, excluding the whole string)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_border
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_border.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* binary representation "0" → no proper border */

    /* length in bits of n (>=1) */
    unsigned len = 64 - __builtin_clzll((uint64_t)n);
    unsigned char bits[64];                /* up to 64-bit numbers */

    for (unsigned i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1ULL);

    return (seq_t)calc_border(bits, len);
}
