/*
 * A133019  Product of n-th prime and n-th prime written backwards.
 * https://oeis.org/A133019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A000040(n) * A004087(n)
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of the n-th prime and its decimal reverse. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static idx_t prime_cache_n   = 0;

/* Reverse the decimal digits of an unsigned 64‑bit integer. */
static seq_t reverse_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0)
    {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0; harmless */

    /* Grow the cached prime list if necessary. */
    if (n > prime_cache_n)
    {
        if (prime_cache)
            free(prime_cache);

        uint32_t *primes;
        size_t cnt;
        sieve_first_n_primes(n, &primes, &cnt);

        prime_cache   = primes;
        prime_cache_n = cnt;
    }

    uint32_t p = prime_cache[n - 1];           /* n-th prime (1‑based) */
    seq_t rev = reverse_uint64(p);            /* reversed digits of p */

    return (seq_t)p * rev;
}
