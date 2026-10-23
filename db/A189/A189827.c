/*
 * A189827  a(n) = d(n-1) + d(n+1), where d(k) is the number of divisors of k.
 * https://oeis.org/A189827
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n-1) + d(n+1) (d(k) = number of divisors of k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* n >= 2 as required by the offset; no need to check bounds */
    uint64_t d1 = divisor_count_u64((uint64_t)(n - 1));
    uint64_t d2 = divisor_count_u64((uint64_t)(n + 1));
    return (seq_t)d1 + (seq_t)d2;
}
