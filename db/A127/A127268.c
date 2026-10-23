/*
 * A127268  If the prime factorization of n is n = Product_{p|n} p^b(p,n) (p =
 *          distinct prime divisors of n, each b(p,n) is a positive integer),
 *          then a(n) is (Sum_{p|n} p^b(p,n)) taken mod (Sum_{p|n} p).
 * https://oeis.org/A127268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑p p^b(p,n)) mod (∑p p) for n>1, a(1)=0 where p ranges
 *            over the distinct prime divisors of n and b(p,n) is the exponent
 *            of p in the prime factorisation of n
 * Range:     u8 n <= 8085, u16 n <= 8085, u32 n <= 8085, u64 n <= 8085,
 *            u128 n <= 8085, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for the OEIS sequence A127268. */
seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0; indices below 1 are unused */
        return 0;

    /* Factorise n, storing distinct primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   k = 0;
    uint64_t m   = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (k < sizeof primes / sizeof primes[0]) {
                primes[k] = p;
                exps[k]   = e;
                ++k;
            }
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        if (k < sizeof primes / sizeof primes[0]) {
            primes[k] = m;
            exps[k]   = 1;
            ++k;
        }
    }

    /* Sum of distinct prime divisors (the modulus). */
    uint64_t sumPrimes = 0;
    for (size_t i = 0; i < k; ++i)
        sumPrimes += primes[i];

    if (sumPrimes == 0)
        return 0;                    /* should not happen for n > 1 */

    /* Sum of prime powers modulo sumPrimes. */
    uint64_t sumVals = 0;
    for (size_t i = 0; i < k; ++i) {
        uint64_t p   = primes[i] % sumPrimes;      /* base modulo modulus */
        uint64_t e   = exps[i];
        uint64_t pow = 1 % sumPrimes;              /* base^0 mod modulus */

        /* Binary exponentiation to compute p^e mod sumPrimes. */
        while (e) {
            if (e & 1)
                pow = (pow * p) % sumPrimes;
            p = (p * p) % sumPrimes;
            e >>= 1;
        }
        sumVals = (sumVals + pow) % sumPrimes;
    }

    return (seq_t)sumVals;
}
