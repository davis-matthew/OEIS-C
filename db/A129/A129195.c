/*
 * A129195  a(n) = denominator(n!/4^n).
 * https://oeis.org/A129195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = denominator(n!/4^n) = 2^(n + popcount(n))
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* popcount of n (number of 1-bits) */
    idx_t pop = __builtin_popcountll(n);
    /* exponent of 2 in the denominator */
    idx_t exponent = n + pop;
    return pow2(exponent);
}
