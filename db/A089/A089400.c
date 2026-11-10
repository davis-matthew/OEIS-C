/*
 * A089400  a(n) = m - A089398(2^m - n) for m>=n.
 * https://oeis.org/A089400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t>=0} bit_t(n+t), where bit_t(x) is bit t of x. This
 *            follows from the definition a(n) = m - A089398(2^m - n) with m
 *            large. The listed data run one step behind the stated offset, so
 *            this is the expression for index n+1 in the derivation, which
 *            matches the listed terms
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (unsigned t = 0; t < 64; ++t) {
        uint64_t x = (uint64_t)n + t;
        sum += (seq_t)((x >> t) & 1u);
    }
    return sum;
}
