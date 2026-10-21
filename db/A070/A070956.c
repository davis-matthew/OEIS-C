/*
 * A070956  Number of pairs (x,y) such that n = gcd(x,y) + lcm(x,y).
 * https://oeis.org/A070956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered pairs (x,y) with n =
 *            gcd(x,y)+lcm(x,y) a(n) = Σ_{d|n, d<n} h( n/d – 1 ) h(1) = 1,
 *            h(t>1) = 2^{ω(t)-1} where ω(t) is the number of distinct prime
 *            factors of t
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   h
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/h.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if (d < n)
            {
                idx_t t = n / d - 1;
                sum += h(t);
            }
            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && d2 < n)
            {
                idx_t t = n / d2 - 1;
                sum += h(t);
            }
        }
    }
    return sum;
}
