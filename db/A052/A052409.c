/*
 * A052409  a(n) = largest integer power m for which a representation of the
 *          form n = k^m exists (for some k).
 * https://oeis.org/A052409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd of the exponents in the prime factorisation of n
 *            (a(1)=0)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9274 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* 1 is a degenerate perfect power with infinite exponent;
           the OEIS defines a(1) = 0. */
        return (seq_t)0;
    }

    uint64_t x = (uint64_t)n;
    uint64_t g = 0;      /* GCD of exponents found so far */
    uint64_t cnt;

    /* handle factor 2 separately */
    cnt = 0;
    while ((x & 1ULL) == 0) {
        ++cnt;
        x >>= 1;
    }
    if (cnt) g = cnt;

    /* factor odd primes by trial division */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        cnt = 0;
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
        if (cnt) {
            g = g ? gcd_u64(g, cnt) : cnt;
        }
    }

    /* if a prime factor greater than sqrt(original n) remains,
       its exponent is 1. */
    if (x > 1) {
        g = g ? gcd_u64(g, 1ULL) : 1ULL;
    }

    return (seq_t)g;
}
