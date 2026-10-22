/*
 * A007404  Decimal expansion of Sum_{n>=0} 1/2^(2^n).
 * https://oeis.org/A007404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) * Sum_{k>=0} 1/2^(2^k)) mod 10. Computed
 *            by building the constant as an exact binary fraction (bits at
 *            positions 2^k) to well beyond the needed precision, then
 *            repeatedly multiplying by 10 and taking the integer carry as the
 *            next digit
 * Range:     u8 n <= 8374, u16 n <= 8374, u32 n <= 8374, u64 n <= 8374,
 *            u128 n <= 8374, big any n
 * Helpers:   digit_with_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_with_bits.h"

seq_t a(idx_t n) {
    uint64_t L = 4 * ((uint64_t)n + 1) + 256;
    int ok = 0;
    int d;
    for (;;) {
        d = digit_with_bits(n, L, &ok);
        if (ok) break;
        L *= 2;
    }
    return (seq_t)d;
}
