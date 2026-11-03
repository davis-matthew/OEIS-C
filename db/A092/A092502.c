/*
 * A092502  First derivative of cyclotomic(n,x) evaluated at x=1.
 * https://oeis.org/A092502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (phi(n)/2) if n has at least two distinct prime factors,
 *            a(p^e) = p * phi(p^e) / 2, a(1) = 1 (phi is Euler’s totient
 *            function)
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 88 terms through n = 4961 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient.h"

/* Determine whether n is a power of a single prime.
   If it is, *prime and *exp are set and the function returns 1.
   Otherwise returns 0. */
static int _is_prime_power(idx_t n, uint64_t *prime, uint64_t *exp)
{
    if (n <= 1) return 0;
    uint64_t num = n;
    uint64_t p, count = 0;
    for (p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            count = 0;
            while (num % p == 0) {
                num /= p;
                ++count;
            }
            *prime = p;
            *exp = count;
            /* if another factor remains, not a prime power */
            if (num > 1) return 0;
            return 1;
        }
    }
    if (num > 1) {
        /* num itself is prime */
        *prime = num;
        *exp = 1;
        return 1;
    }
    return 0;   /* n==1 handled above */
}

/* Compute gcd of two uint64_t values (used only for safety in phi) */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute totient of n */
    seq_t phi = totient(n);

    /* Check whether n is a prime power */
    uint64_t p, e;
    if (_is_prime_power(n, &p, &e)) {
        /* a(p^e) = p * phi(p^e) / 2 */
        return ((seq_t)p * phi) / 2;
    } else {
        /* a(n) = phi(n) / 2 for composite n with ≥2 distinct primes */
        return phi / 2;
    }
}
