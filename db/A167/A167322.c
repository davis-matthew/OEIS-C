/*
 * A167322  Totally multiplicative sequence with a(p) = 3*(p+3) for prime p.
 * https://oeis.org/A167322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = \prod_{p^e\|n} (3 \cdot (p + 3))^e
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p;

        /* Find smallest prime factor */
        if ((temp & 1) == 0) {
            p = 2;
        } else {
            p = 3;
            while (p * p <= temp && temp % p != 0) {
                p += 2;
            }
            if (p * p > temp) {
                p = temp; /* remaining temp is prime */
            }
        }

        /* Count exponent of p */
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        /* Compute factor (3*(p+3)) and raise to exp */
        seq_t factor = (seq_t)3 * (seq_t)(p + 3);
        for (uint64_t i = 0; i < exp; ++i) {
            result *= factor;
        }
    }

    return result;
}
