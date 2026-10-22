/*
 * A000006  Integer part of square root of n-th prime.
 * https://oeis.org/A000006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000196(A000040(n)) = floor(sqrt(prime(n))); primes found
 *            by a sieve of Eratosthenes, then integer square root
 * Range:     u8 n <= 6542, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_u64.h"

static uint64_t *primes_cache = NULL;
static uint64_t primes_count = 0;
static uint64_t sieve_limit = 0;

static void build_primes(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cnt = 0;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i)
                comp[j] = 1;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) cnt++;
    free(primes_cache);
    primes_cache = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) primes_cache[cnt++] = i;
    primes_count = cnt;
    sieve_limit = limit;
    free(comp);
}

static uint64_t nth_prime(uint64_t n)
{
    uint64_t limit = sieve_limit ? sieve_limit : 1024;
    while (primes_count < n) {
        if (sieve_limit) limit *= 2;
        build_primes(limit);
    }
    return primes_cache[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime((uint64_t)n);
    return (seq_t)isqrt_u64(p);
}
