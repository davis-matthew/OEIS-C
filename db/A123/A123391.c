/*
 * A123391  a(n) = sum of exponents that are primes in the prime factorization
 *          of n.
 * https://oeis.org/A123391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of exponents that are primes in the prime
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* small helper: test if a 64‑bit integer x is a prime (x ≤ 64 in practice) */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = (uint64_t)n;
    seq_t sum = 0;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (is_prime_small(exp))
                sum += (seq_t)exp;
        }
    }

    /* temp > 1 ⇒ remaining prime factor with exponent 1 (not prime) */
    return sum;
}
