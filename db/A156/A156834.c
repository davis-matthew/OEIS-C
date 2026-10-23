/*
 * A156834  A156348 * A000010.
 * https://oeis.org/A156834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} phi(d) * binomial(d + n/d - 2, d - 1)
 * Range:     u8 n <= 19, u16 n <= 69, u32 n <= 265, u64 n <= 1022,
 *            u128 n <= 4031, big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Assume the following headers provide the needed utility functions. */
#include "../utils/phi.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Enumerate divisors of n via trial division up to sqrt(n). */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* First divisor: d */
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* Compute term for divisor d1 */
            seq_t phi1 = phi(d1);
            idx_t n_over_d1 = n / d1;
            idx_t binom_numerator = d1 + n_over_d1 - 2;
            idx_t binom_denominator = d1 - 1;
            seq_t binom1 = binomial(binom_numerator, binom_denominator);
            result += phi1 * binom1;

            /* If d2 is different, compute term for divisor d2 */
            if (d2 != d1) {
                seq_t phi2 = phi(d2);
                idx_t n_over_d2 = n / d2; /* which equals d1 */
                idx_t binom_num2 = d2 + n_over_d2 - 2;
                idx_t binom_den2 = d2 - 1;
                seq_t binom2 = binomial(binom_num2, binom_den2);
                result += phi2 * binom2;
            }
        }
    }
    return result;
}
