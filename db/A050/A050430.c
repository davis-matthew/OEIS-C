/*
 * A050430  Length of longest palindromic subword of (n base 2).
 * https://oeis.org/A050430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest palindromic subword of the binary
 *            representation of n
 * Range:     u8 n <= 9206, u16 n <= 9206, u32 n <= 9206, u64 n <= 9206,
 *            u128 n <= 9206, big any n
 * Helpers:   longest_pal_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/longest_pal_binary.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used for OEIS offset 1 but safe */

    /* Determine number of bits in n. */
    idx_t len = 0;
    uint64_t tmp = n;
    while (tmp) { ++len; tmp >>= 1; }

    /* Extract bits into a static array (max ~64 bits for uint64_t). */
    uint8_t bits[128];
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1ULL);
    }

    return longest_pal_binary(bits, len);
}
