/*
 * A082051  Sum of divisors of n that are not of the form 3k+2.
 * https://oeis.org/A082051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d mod 3 != 2} d
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is: sum of all divisors of n that are NOT congruent to 2 modulo 3 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate up to sqrt(n) */
        if (n % d == 0) {
            /* divisor d */
            if ((d % 3) != 2) {
                sum += (seq_t)d;
            }
            /* paired divisor e = n / d */
            idx_t e = n / d;
            if (e != d && (e % 3) != 2) {
                sum += (seq_t)e;
            }
        }
    }
    return sum;
}
