/*
 * A000188  (1) Number of solutions to x^2 == 0 (mod n). (2) Also square root
 *          of largest square dividing n. (3) Also max_{ d divides n } gcd(d,
 *          n/d).
 * https://oeis.org/A000188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Product over p^e || n of p^floor(e/2) (multiplicative,
 *            a(p^e) = p^floor(e/2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    if (m == 0) return r;
    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            while (e >= 2) {
                r *= (seq_t)p;
                e -= 2;
            }
        }
    }
    /* remaining m is 1 or a prime with exponent 1: contributes nothing */
    return r;
}
