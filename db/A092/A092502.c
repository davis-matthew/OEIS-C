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
 * Helpers:   _is_prime_power, _phi_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_is_prime_power.h"
#include "../utils/_phi_value.h"

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
    seq_t phi = _phi_value(n);

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
