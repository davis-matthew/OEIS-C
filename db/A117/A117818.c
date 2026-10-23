/*
 * A117818  a(n) = n if n is 1 or a prime, otherwise a(n) = n divided by the
 *          least prime factor of n (A032742(n)).
 * https://oeis.org/A117818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is 1 or prime, otherwise a(n) = n divided by the
 *            least prime factor of n (A032742(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t nn = n;
    for (uint64_t i = 2; i <= nn / i; ++i) {
        if (nn % i == 0) {
            return (seq_t)(nn / i);
        }
    }
    return (seq_t)nn;
}
