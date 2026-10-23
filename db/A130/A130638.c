/*
 * A130638  a(n) = 1 iff d(n) = d(n+1), otherwise a(n)=0, where d(n) is the
 *          number of divisors of n, A000005.
 * https://oeis.org/A130638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if divisor_count(n) == divisor_count(n+1) else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t dn  = divisor_count_u64((uint64_t)n);
    uint64_t dn1 = divisor_count_u64((uint64_t)(n + 1));

    return (seq_t)(dn == dn1);
}
