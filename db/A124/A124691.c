/*
 * A124691  a(n) = lcm(prime(n)+1, prime(n+1)+1) / 2.
 * https://oeis.org/A124691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(prime(n)+1, prime(n+1)+1) / 2
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

#define PRIME_SIEVE_LIMIT 200000

static uint32_t *prime_cache = NULL;
static size_t prime_cache_count = 0;

/* Build a small sieve of primes for fast access up to PRIME_SIEVE_LIMIT. */
static void init_primes(void)
{
    if (prime_cache != NULL)
        return;

    uint8_t *mark = (uint8_t *)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!mark)
        return;

    mark[0] = mark[1] = 0;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                mark[j] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (mark[i])
            ++cnt;

    prime_cache = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_cache) {
        free(mark);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (mark[i])
            prime_cache[idx++] = (uint32_t)i;

    prime_cache_count = cnt;
    free(mark);
}

/* Retrieve the k‑th prime, using the cache when possible. */
static seq_t cached_prime(idx_t k)
{
    if ((size_t)k <= prime_cache_count)
        return (seq_t)prime_cache[(size_t)k - 1];
    /* Fallback to the generic prime routine. */
    return (seq_t)get_prime(k);
}

/* Sequence a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    init_primes();

    seq_t p1 = cached_prime(n);
    seq_t p2 = cached_prime(n + 1);

    seq_t v1 = p1 + 1;
    seq_t v2 = p2 + 1;

    seq_t g   = gcd(v1, v2);
    seq_t lcm = (v1 / g) * v2;

    return lcm / 2;
}
