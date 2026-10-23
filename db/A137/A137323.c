/*
 * A137323  a(n) = sum(d divides n, 2^(n/d-1) - 1 ), omitting d=1 and d=n.
 * https://oeis.org/A137323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{d|n, 1<d<n} ( 2^(n/d-1) - 1 )
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over proper divisors d > 1 */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            /* contribution for divisor d */
            idx_t exp1 = n / d - 1;
            seq_t term = pow2(exp1);
            term -= 1;
            sum += term;

            /* contribution for the complementary divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t exp2 = d - 1;
                term = pow2(exp2);
                term -= 1;
                sum += term;
            }
        }
    }
    return sum;
}
