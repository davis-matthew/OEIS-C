/*
 * A218403  Bitwise OR of all proper divisors of n; a(1) = 0 by convention.
 * https://oeis.org/A218403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = bitwise OR of all proper divisors of n (a(1)=0 by
 *            convention)
 * Range:     u8 n <= 511, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t res = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* d is a divisor of n */
            if (d < n)
                res |= (seq_t)d;

            /* the paired divisor e = n/d */
            idx_t e = n / d;
            if (e != d && e < n)
                res |= (seq_t)e;
        }
    }
    return res;
}
