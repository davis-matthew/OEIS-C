/*
 * A051085  Primes p such that x^32 = -2 has a solution mod p.
 * https://oeis.org/A051085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p such that the congruence x^32 ≡ -2 (mod
 *            p) has a solution. This holds iff (p-2)^{(p-1)/gcd(32,p-1)} ≡ 1
 *            (mod p)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple incremental prime generator and fast modular exponentiation. */

#include <stdint.h>
#include <stdlib.h>

/* fast modular exponentiation using 128-bit intermediate */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* returns the n-th prime that satisfies the condition */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but not used */

    /* dynamic array of found primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    if (primes == NULL) {
        primes_cap = 1024;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    uint64_t satisfied = 0;   /* number of primes meeting the condition so far */
    uint64_t candidate = 2;   /* start from the first prime */

    while (1) {
        /* primality test: trial division by known primes */
        int is_prime = 1;
        if (candidate == 2) {
            /* 2 is prime */
        } else if ((candidate & 1U) == 0) {
            is_prime = 0;
        } else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
        }

        if (is_prime) {
            /* store the new prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = candidate;

            /* check the condition */
            int satisfies = 0;
            if (candidate == 2) {
                satisfies = 1;
            } else {
                uint64_t p = candidate;
                uint64_t v = p - 1;

                /* count trailing zeros of p-1 to get gcd(32,p-1) */
                int v2 = 0;
                uint64_t temp = v;
                while ((temp & 1U) == 0) {
                    ++v2;
                    temp >>= 1;
                }
                int shift = v2 < 5 ? v2 : 5;
                uint64_t d = 1ULL << shift;           /* gcd(32,p-1) */
                uint64_t e = (p - 1) / d;             /* exponent for test */

                if (mod_pow64(p - 2, e, p) == 1ULL)
                    satisfies = 1;
            }

            if (satisfies) {
                ++satisfied;
                if (satisfied == n)
                    return (seq_t)candidate;
            }
        }

        ++candidate;
    }
}
