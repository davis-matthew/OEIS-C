/*
 * A105891  Primes for which -19 is a primitive root.
 * https://oeis.org/A105891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p for which −19 is a primitive
 *   root modulo p (OEIS A105891). The sequence starts with
 *   a(1)=2, a(2)=3, a(3)=13, …              */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/simple_sieve.h"

/* ----- modular exponentiation (base, exp, mod < 2^64) ----- */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* ----- test whether −19 is a primitive root modulo p ----- */
static bool is_primitive_root(uint32_t p, const uint32_t *primes,
                              size_t prime_count)
{
    if (p == 19) return false;          /* base ≡ 0 (mod 19) – not a unit */
    uint32_t base = (p - (19 % p));     /* (−19) mod p, always < p */

    uint32_t m = p - 1;                  /* φ(p) = p−1 for prime p */
    uint32_t temp = m;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= temp;
         ++i) {
        if (temp % primes[i] == 0) {
            uint32_t q = primes[i];
            while (temp % q == 0) temp /= q;
            if (modpow64((uint64_t)base, (uint64_t)(m / q), (uint64_t)p)
                == 1) {
                return false;
            }
        }
    }
    if (temp > 1) {                      /* remaining prime factor */
        if (modpow64((uint64_t)base, (uint64_t)(m / temp),
                     (uint64_t)p) == 1) {
            return false;
        }
    }
    return true;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, defensive */

    /* Rough upper bound: n * 400 + 1 000 guarantees enough primes
       for all requested n up to about 10 000. */
    uint32_t limit = (uint32_t)(n * 400ULL + 1000ULL);
    if (limit < 1000) limit = 1000;

    seq_t result = 0;

    for (;;) {
        uint32_t *primes;
        size_t prime_count;
        simple_sieve(limit, &primes, &prime_count);
        if (!primes) break;             /* allocation failure – abort */

        size_t found = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = primes[i];
            if (is_primitive_root(p, primes, prime_count)) {
                ++found;
                if (found == (size_t)n) {
                    result = (seq_t)p;
                    free(primes);
                    return result;
                }
            }
        }
        free(primes);
        limit <<= 1;                    /* double the sieve limit and try again */
    }
    return result; /* should never reach here */
}
