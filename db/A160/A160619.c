/*
 * A160619  a(n) = Sum_{d|n} phi(n/d)*2^(d+1), with a(0) = 0.
 * https://oeis.org/A160619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} φ(n/d)*2^(d+1) , a(0)=0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t result = (seq_t)0;

    /* iterate over divisors d of n with d <= n/d to avoid overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) {
            continue;
        }

        idx_t d1 = d;            /* divisor */
        idx_t d2 = n / d;        /* paired divisor */

        /* term for divisor d1 */
        seq_t term1 = phi(n / d1) * pow2(d1 + 1);
        result += term1;

        if (d1 != d2) {
            /* term for divisor d2 (only if distinct) */
            seq_t term2 = phi(n / d2) * pow2(d2 + 1);
            result += term2;
        }
    }
    return result;
}
