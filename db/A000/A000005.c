/*
 * A000005  d(n) (also called tau(n) or sigma_0(n)), the number of divisors of
 *          n.
 * https://oeis.org/A000005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice, mult, hear
 * Formula:   a(n) = Product_{p^e || n} (e+1) (multiplicative, a(p^e) = e+1)
 * Range:     u8 n <= 9906, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;

    if (m == 0)
        return (seq_t)0;

    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= (seq_t)(e + 1);
        }
    }
    if (m > 1)
        result *= 2;
    return result;
}
