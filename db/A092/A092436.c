/*
 * A092436  a(n) = 1/2 + (-1)^n*(1/2 - A010060(floor(n/2))).
 * https://oeis.org/A092436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - A010060(n) where A010060(n) is the Thue-Morse
 *            sequence (parity of popcount of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity64.h"

/* a(n) = 1 - parity(n), defined for n >= 1. */
seq_t a(idx_t n)
{
    return (seq_t)(1 - parity64(n));
}
