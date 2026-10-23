/*
 * A116891  a(n) = gcd(n! + 1, n^n + 1).
 * https://oeis.org/A116891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n! + 1, nⁿ + 1)
 * Range:     u8 n <= 130, u16 n <= 9488, u32 n <= 9488, u64 n <= 9488,
 *            u128 n <= 9488, big any n
 * Helpers:   fact_mod_uint32, powmod_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fact_mod_uint32.h"
#include "../utils/powmod_uint32.h"

/* Limit for the sieve that is sufficient for n ≤ 10000 (6n+100 ≤ 61000) */
#define PRIME_LIMIT 65000

/* Sieve data structures */
static bool sieve_composite[PRIME_LIMIT + 1];
static uint32_t primes[7000];
static unsigned int prime_count;
static bool primes_sieved = false;

/* Simple Sieve of Eratosthenes – executed only once */
static void sieve_primes(void)
{
    if (primes_sieved) return;
    primes_sieved = true;
    prime_count = 0;
    for (int i = 0; i <= PRIME_LIMIT; ++i)
        sieve_composite[i] = false;
    sieve_composite[0] = sieve_composite[1] = true;

    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!sieve_composite[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve_composite[j] = true;
        }
    }

    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve_composite[i]) {
            primes[prime_count++] = (uint32_t)i;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 2 explicitly */
    if (n == 1) return (seq_t)2;

    sieve_primes();            /* ensure primes are ready */

    /* Search for primes p > n up to a safety bound (6n+100) */
    uint32_t limit = (uint32_t)(6 * n + 100);
    if (limit > (uint32_t)PRIME_LIMIT) limit = (uint32_t)PRIME_LIMIT;

    for (unsigned int i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if (p <= (uint32_t)n) continue;
        if (p > limit) break;

        /* Check n! ≡ –1 (mod p) and nⁿ ≡ –1 (mod p) */
        if (fact_mod_uint32((uint32_t)n, p) == p - 1u &&
            powmod_uint32((uint32_t)n, (uint32_t)n, p) == p - 1u)
            return (seq_t)p;      /* gcd is the prime p */
    }

    /* No common prime factor found: gcd = 1 */
    return (seq_t)1;
}
