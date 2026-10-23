/*
 * A125030  a(n) = sum of exponents in the prime factorization of n that are
 *          noncomposite.
 * https://oeis.org/A125030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e‖n} e (over all prime factors p of n) where the
 *            exponent e is added only if e == 1 or e is prime (i.e.
 *            non‑composite)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"   /* provides _smallest_prime_factor(uint64_t) */

seq_t a(idx_t n)
{
    /* a(1) is 0 by definition */
    if (n < 1) return (seq_t)0;

    seq_t result = 0;
    uint64_t m = (uint64_t)n;          /* work on a 64‑bit copy for factorisation */

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m); /* smallest prime factor of m */
        uint64_t exp = 0;

        /* count the exponent of p in the prime factorisation of m */
        do {
            m /= p;
            ++exp;
        } while (m % p == 0);

        /* add the exponent if it is 1 or a prime */
        if (exp == 1 || _smallest_prime_factor(exp) == exp)
            result += (seq_t)exp;
    }

    return result;
}
