/*
 * A010553  a(n) = tau(tau(n)).
 * https://oeis.org/A010553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(tau(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t1 = divisor_count_u64((uint64_t)n);
    seq_t t2 = divisor_count_u64((uint64_t)t1);
    return t2;
}
