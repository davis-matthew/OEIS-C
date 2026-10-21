/*
 * A065915  Numerator of sigma(8*n^2)/sigma(4*n^2).
 * https://oeis.org/A065915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(2*v2(n)+4) - 1 where v2(n) is the exponent of 2 in n
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Compute the exponent of 2 dividing n (trailing zeros of n) */
static inline uint64_t count_trailing_zeros(uint64_t x) {
    uint64_t tz = 0;
    while ((x & 1ULL) == 0) {
        ++tz;
        x >>= 1;
    }
    return tz;
}

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 by the OEIS specification */
    uint64_t ux = (uint64_t)n;
    uint64_t tz = count_trailing_zeros(ux);          /* v2(n) */
    idx_t exp = (idx_t)(2 * tz + 4);                 /* exponent of 2 */
    seq_t val = pow2(exp);                           /* 2^exp as seq_t */
    val -= (seq_t)1;                                 /* subtract 1 */
    return val;
}
