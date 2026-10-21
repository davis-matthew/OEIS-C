/*
 * A056624  Number of unitary square divisors of n.
 * https://oeis.org/A056624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = 2^(# of distinct prime factors of n whose exponent is
 *            even)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t res = 1;

    if (m <= 1) return res;

    /* handle factor 2 separately to allow step through odds only */
    if ((m & 1ULL) == 0) {
        unsigned e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        if ((e & 1U) == 0) {
            res *= (seq_t)2;
        }
    }

    /* loop over odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if ((e & 1U) == 0) {
                res *= (seq_t)2;
            }
        }
    }

    /* if a prime > sqrt(n) remains, its exponent is 1 (odd) => no change */
    if (m > 1) {
        /* m is prime with exponent 1; odd exponent => nothing to do */
    }

    return res;
}
