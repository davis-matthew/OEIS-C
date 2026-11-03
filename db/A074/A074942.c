/*
 * A074942  a(n) = phi(n) mod 3.
 * https://oeis.org/A074942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = phi(n) mod 3
 * Range:     u8 n <= 1299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9231 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

seq_t a(idx_t n)
{
    /* Euler's totient φ(n) can be computed by the helper phi().
       We simply return the remainder after division by 3. */
    seq_t phi_val = phi(n);
    return phi_val % (seq_t)3;
}
