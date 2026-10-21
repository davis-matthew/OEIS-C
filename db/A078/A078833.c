/*
 * A078833  Greatest prime contained as binary substring in binary
 *          representation of n>1, a(1)=1.
 * https://oeis.org/A078833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest prime represented by any contiguous binary substring of n
   (with a(1) = 1). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;           /* first term of the sequence */

    /* Determine the length of the binary representation of n. */
    idx_t tmp = n;
    unsigned int len = 0;
    while (tmp) {
        tmp >>= 1;
        ++len;
    }

    /* Search all contiguous substrings and keep the largest prime. */
    idx_t best = 0;
    for (unsigned int i = 0; i < len; ++i) {
        idx_t val = 0;
        for (unsigned int j = i; j < len; ++j) {
            const idx_t bit = (n >> (len - 1 - j)) & (idx_t)1;
            val = (val << 1) | bit;
            if (val > best && is_prime_big(val))
                best = val;
        }
    }

    return (seq_t)best;
}
