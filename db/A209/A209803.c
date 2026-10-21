/*
 * A209803  a(n) = Sum_{d|n} d*2^(n*d).
 * https://oeis.org/A209803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * 2^(n*d)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

/* Compute a(n) for n >= 1.  The function works for all supported
 * types seq_t and idx_t.  It iterates over all divisors d of n and
 * accumulates d * 2^(n*d).  For n = 0 it returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1, but 0 is harmless */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t d1 = d;
            seq_t pow1 = pow_uint(2, n * d1);
            sum += mul_small(pow1, (unsigned)d1);

            /* second divisor n/d, distinct from d when n is not a square */
            if (d1 != n / d) {
                idx_t d2 = n / d;
                seq_t pow2 = pow_uint(2, n * d2);
                sum += mul_small(pow2, (unsigned)d2);
            }
        }
    }

    return sum;
}
