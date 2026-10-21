/*
 * A080214  Binomial(greatest prime factor of n, smallest prime factor of n).
 * https://oeis.org/A080214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = binomial(greatest prime factor of n, smallest prime
 *            factor of n)
 * Range:     u8 n <= 38, u16 n <= 144, u32 n <= 472, u64 n <= 1816,
 *            u128 n <= 6682, big any n
 * Helpers:   binomial_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_idx.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                  /* a(1) = 1, a(0) not used */
    uint64_t m = (uint64_t)n;
    uint64_t lpf = 0, gpf = 0;
    uint64_t temp = m;

    /* factor out 2 first */
    if (temp % 2 == 0) {
        lpf = 2;
        do { temp /= 2; } while (temp % 2 == 0);
        gpf = 2;
    }

    /* iterate over odd divisors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (lpf == 0) lpf = p;
            do { temp /= p; } while (temp % p == 0);
            gpf = p;          /* keep the largest seen so far */
        }
    }

    /* remaining factor > 1 is prime */
    if (temp > 1) {
        if (lpf == 0) lpf = temp;
        gpf = temp;
    }

    return binomial_idx(gpf, lpf);
}
