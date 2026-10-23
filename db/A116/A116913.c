/*
 * A116913  Inverse Moebius transform of pentagonal numbers.
 * https://oeis.org/A116913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d*(3*d - 1)/2
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t nn = n;

    /* iterate through all divisors d of n, with d <= n/d to avoid duplicates */
    for (idx_t d = 1; d <= nn / d; ++d) {
        if (nn % d == 0) {
            seq_t dd = (seq_t)d;
            /* term = d*(3*d - 1)/2 */
            seq_t term = dd * ((seq_t)3 * dd - 1) / 2;
            sum += term;

            idx_t other = nn / d;
            if (other != d) {
                seq_t oo = (seq_t)other;
                seq_t term2 = oo * ((seq_t)3 * oo - 1) / 2;
                sum += term2;
            }
        }
    }
    return sum;
}
