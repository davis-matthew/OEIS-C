/*
 * A091392  Product_{ p | n } (1 + Legendre(-2,p) ).
 * https://oeis.org/A091392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p | n} (1 + Legendre(-2, p))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modpow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where the Legendre symbol is defined for odd primes p and Legendre(-2,2)=0 */
/* The product runs over distinct prime divisors of n.  */

/* Implementation note:  the function uses trial division factoring of n
 * and a fast modular exponentiation to compute Legendre(-2, p).  All
 * intermediate arithmetic is performed with __uint128_t to avoid 64‑bit
 * overflow when multiplying by 2.  The result is returned as seq_t.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/modpow64.h"

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2ULL; p * p <= temp; ++p) {
        if (temp % p == 0ULL) {
            /* distinct prime divisor found; remove all copies */
            while (temp % p == 0ULL)
                temp /= p;

            if (p == 2ULL) {
                /* Legendre(-2,2)=0, factor 1 – do nothing */
                continue;
            }
            /* Compute Legendre(-2, p) using Euler's criterion */
            uint64_t t = modpow64((p == 2ULL ? 0ULL : p - 2ULL), (p - 1ULL) / 2ULL, p);
            if (t == 1ULL) {
                /* Legendre = +1 -> factor 2 */
                result = (seq_t)(((__uint128_t)result) * 2ULL);
            } else {
                /* Legendre = -1 -> factor 0, product becomes 0 */
                result = (seq_t)0;
                return result;
            }
        }
    }

    /* If remaining temp > 1, it is a prime factor > sqrt(original n) */
    if (temp > 1ULL) {
        uint64_t p = temp;
        if (p > 2ULL) {
            uint64_t t = modpow64(p - 2ULL, (p - 1ULL) / 2ULL, p);
            if (t == 1ULL) {
                result = (seq_t)(((__uint128_t)result) * 2ULL);
            } else {
                result = (seq_t)0;
                return result;
            }
        }
        /* p == 2 handled by factor 1, nothing to do */
    }

    return result;
}
