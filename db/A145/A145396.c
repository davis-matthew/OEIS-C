/*
 * A145396  a(n) = Sum_{d|n} sigma(d) + 3*Sum_{2c|n} sigma(c).
 * https://oeis.org/A145396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} sigma(d) + 3*Sum_{2c|n} sigma(c) (where
 *            sigma(k) = sum of divisors of k)
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the sum of divisors of m. */
static seq_t sigma_of(idx_t m)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            sum += (seq_t)d;
            idx_t other = m / d;
            if (other != d)
                sum += (seq_t)other;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum1 = 0;   /* Sum_{d|n} sigma(d) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum1 += sigma_of(d);
            idx_t other = n / d;
            if (other != d)
                sum1 += sigma_of(other);
        }
    }

    if ((n & 1) == 0) {   /* n even: add 3*Sum_{c|n/2} sigma(c) */
        idx_t half = n >> 1;
        seq_t sum2 = 0;
        for (idx_t d = 1; d * d <= half; ++d) {
            if (half % d == 0) {
                sum2 += sigma_of(d);
                idx_t other = half / d;
                if (other != d)
                    sum2 += sigma_of(other);
            }
        }
        return sum1 + ((seq_t)3 * sum2);
    } else {
        return sum1;
    }
}
