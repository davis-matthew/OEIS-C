/*
 * A097017  a(n) = sigma(5*n) modulo 6.
 * https://oeis.org/A097017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(5*n) mod 6 where sigma(m) is the sum of all
 *            divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute sigma(5*n) modulo 6 without large intermediate values. */
    idx_t m = 5 * n;      /* divisible by 5, fits in idx_t */
    unsigned int sum_mod = 0;   /* hold result modulo 6 (0..5) */

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* add divisor d */
            sum_mod += (unsigned)(d % 6);
            if (sum_mod >= 6) sum_mod -= 6;

            /* add complementary divisor m/d if distinct */
            idx_t dd = m / d;
            if (dd != d) {
                sum_mod += (unsigned)(dd % 6);
                if (sum_mod >= 6) sum_mod -= 6;
            }
        }
    }
    return (seq_t)sum_mod;
}
