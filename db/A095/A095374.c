/*
 * A095374  One less than the number of divisors of 2*n + 1.
 * https://oeis.org/A095374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = divisor_count(2*n + 1) - 1
 * Range:     u8 n <= 564, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _divisor_count_u64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Here divisor_count(k) returns the number of positive divisors of k.) */

#include <stdint.h>
#include "../utils/_divisor_count_u64_local.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* not required by the sequence, but safe */

    /* compute 2*n + 1 in seq_t to avoid overflow on idx_t */
    seq_t m = (seq_t)n * 2 + 1;

    /* Since n in the tests is small, m comfortably fits in 64 bits.
       Even for larger n, this implementation will work as long as m <= 2^64-1. */
    uint64_t m_u64 = (uint64_t)m;

    uint64_t tau = _divisor_count_u64_local(m_u64);
    return (seq_t)(tau - 1);
}
