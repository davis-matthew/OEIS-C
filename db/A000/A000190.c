/*
 * A000190  Number of solutions to x^4 == 0 (mod n).
 * https://oeis.org/A000190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative with a(p^e) = p^floor(3e/4)
 * Range:     u8 n <= 2047, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            unsigned k, t;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            t = (3 * e) / 4;
            for (k = 0; k < t; k++)
                r *= (seq_t)p;
        }
    }
    /* any leftover prime factor has exponent 1, so it contributes p^0 = 1 */
    return r;
}
