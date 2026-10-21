/*
 * A088978  Number of Pythagorean triangles having the n-th prime prime(n) as
 *          one of their sides.
 * https://oeis.org/A088978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(n) = 1 + [prime(n) % 4 == 1] (for n > 1) Where
 *            prime(n) is the n-th prime. Thus every odd prime contributes one
 *            Pythagorean triple (as odd leg), and if it is 1 (mod 4) it
 *            contributes an additional triple (as hypotenuse)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum number of primes pre‑sieved. */
#define PRIMES_REQUIRED 12000
/* Upper limit for the sieve to obtain at least PRIMES_REQUIRED primes. */
#define SIEVE_LIMIT     200000

/* Pre‑compute the first PRIMES_REQUIRED primes with a standard sieve. */
static void init_primes(uint32_t *primes, uint32_t *cnt)
{
    static bool done = false;
    static uint8_t sieve[SIEVE_LIMIT + 1];

    if (done)
        return;

    /* Initialise sieve: 1 = prime candidate */
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 0;

    *cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT && *cnt < PRIMES_REQUIRED; ++i)
        if (sieve[i])
            primes[++(*cnt)] = i;

    done = true;
}

/* Compute a(idx_t n) as defined above. */
seq_t a(idx_t n)
{
    static uint32_t primes[PRIMES_REQUIRED];
    static uint32_t prime_cnt = 0;

    if (prime_cnt == 0)
        init_primes(primes, &prime_cnt);

    if (n <= 1)
        return (seq_t)0;

    if (n <= prime_cnt)
    {
        uint32_t p = primes[(uint32_t)n];   /* array is 1‑based after init */
        return (p % 4U == 1U) ? (seq_t)2 : (seq_t)1;
    }

    /* Fallback: find the n‑th prime by simple trial division using the
       primes we already know.  This keeps the implementation simple
       while still being fast enough for n up to ~10⁵. */
    uint32_t found = prime_cnt;
    uint32_t candidate = (prime_cnt > 0 ? primes[prime_cnt] + 2 : 3);

    while (found < n)
    {
        bool is_prime = true;
        for (uint32_t i = 1; i <= prime_cnt; ++i)
        {
            uint32_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            ++found;
            if (found == n)
                return (candidate % 4U == 1U) ? (seq_t)2 : (seq_t)1;
        }

        candidate += 2; /* skip even numbers */
    }

    /* Should never reach here. */
    return (seq_t)0;
}
