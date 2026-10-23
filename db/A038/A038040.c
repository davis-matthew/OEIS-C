/*
 * A038040  a(n) = n*d(n), where d(n) = number of divisors of n (A000005).
 * https://oeis.org/A038040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n*d(n) (d(n) = number of divisors of n)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    uint64_t d = divisor_count_u64((uint64_t)n);
    return (seq_t)n * (seq_t)d;
}
