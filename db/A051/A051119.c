/*
 * A051119  a(n) = n/p^k, where p = largest prime dividing n and p^k = highest
 *          power of p dividing n.
 * https://oeis.org/A051119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n / p^k, where p is the largest prime divisor of n and
 *            p^k is the highest power of p dividing n. Computation: find the
 *            largest prime factor p with helper gpf(), then repeatedly divide
 *            n by p until it is no longer divisible. The remaining quotient
 *            is a(n). Complexity: dominated by the facto...
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, a(0) is unused but safe */
        return (seq_t)n;

    uint64_t value = n;
    uint64_t p = gpf(value);          /* largest prime divisor of n */
    if (p == 0)                       /* defensive: shouldn't happen for n>1 */
        return (seq_t)value;

    while (value % p == 0)
        value /= p;

    return (seq_t)value;
}
