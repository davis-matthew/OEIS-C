/*
 * A145094  Coefficients in expansion of Eisenstein series q*E'_4.
 * https://oeis.org/A145094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 240 * n * Σ_{d|n} d^3 (OEIS A145094)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 63, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, return 0 */

    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* cube of the divisor d */
            seq_t d_cube = (seq_t)d * (seq_t)d * (seq_t)d;
            sum += d_cube;

            idx_t d2 = n / d;
            if (d2 != d) {
                /* cube of the paired divisor n/d */
                seq_t d2_cube = (seq_t)d2 * (seq_t)d2 * (seq_t)d2;
                sum += d2_cube;
            }
        }
    }

    /* a(n) = 240 * n * Σ d^3 */
    seq_t result = (seq_t)240 * (seq_t)n * sum;
    return result;
}
