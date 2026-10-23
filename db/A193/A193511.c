/*
 * A193511  a(n) = Sum of even divisors of Omega(n), a(1) = 0.
 * https://oeis.org/A193511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of even divisors of Omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n <= 1ULL)
        return 0;

    seq_t omega = bigomega(n);
    seq_t sum   = 0;

    for (seq_t d = 2; d <= omega; ++d)
    {
        if ((omega % d == 0) && ((d % 2) == 0))
            sum += d;
    }

    return sum;
}
