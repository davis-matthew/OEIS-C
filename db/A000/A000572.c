/*
 * A000572  A Beatty sequence: [ n(e+1) ].
 * https://oeis.org/A000572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(e+1)) = 3n + floor(n*(e-2)), where n*(e-2) =
 *            Sum_{k>=2} n/k! is evaluated exactly as a mixed-radix
 *            (factorial-base) number with K = n+2 digits; the truncated tail
 *            is < 1 unit of the last digit, so the floor is exact
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t K = (uint64_t)n + 2;
    uint64_t carry = 0;
    uint64_t k;
    for (k = K; k >= 2; k--) {
        uint64_t d = (uint64_t)n + carry;
        carry = d / k;
    }
    return (seq_t)n * 3 + (seq_t)carry;
}
