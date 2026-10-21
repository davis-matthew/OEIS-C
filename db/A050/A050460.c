/*
 * A050460  a(n) = Sum_{d|n, n/d=1 mod 4} d.
 * https://oeis.org/A050460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, n/d=1 mod 4} d
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t q = n / d;
            if (q % 4 == 1)
                sum += (seq_t)d;
            if (d % 4 == 1 && d != q)
                sum += (seq_t)q;
        }
    }
    return sum;
}
