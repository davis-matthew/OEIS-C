/*
 * A054662  Number of monic irreducible polynomials over GF(5) with fixed
 *          nonzero trace.
 * https://oeis.org/A054662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/(5*n) * Sum_{d|n, gcd(d,5)=1} mu(d) * 5^(n/d)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mobius, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;          /* d not a divisor of n   */
        if (d % 5 == 0) continue;          /* gcd(d,5) != 1          */
        seq_t term = pow_int((seq_t)5, n / d);
        int mu = (int)mobius(d);
        if (mu > 0)
            sum_pos += term;
        else if (mu < 0)
            sum_neg += term;
    }
    seq_t sum = sum_pos - sum_neg;          /* sum_pos >= sum_neg       */
    seq_t denom = (seq_t)5 * (seq_t)n;
    return sum / denom;
}
