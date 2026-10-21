/*
 * A079079  a(n) = (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+1)*(prime(n+1)+1)/4 (OEIS A079079)
 * Range:     u8 n <= 10, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_limit_estimate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/prime_limit_estimate.h"

/* static cache of primes */
static uint32_t *prime_cache = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* initialise the prime sieve up to at least the index needed */
static void init_primes(idx_t need_index)
{
    if (prime_cache != NULL && prime_count >= (size_t)need_index)
        return;   /* already enough */

    /* determine required limit */
    size_t limit = prime_limit_estimate((idx_t)(need_index + 10));
    if (limit < 100) limit = 100;

    /* allocate sieve array */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* allocate prime array */
    if (prime_cache == NULL)
        prime_cache = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    else
        prime_cache = (uint32_t *)realloc(prime_cache, cnt * sizeof(uint32_t));

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_cache[idx++] = (uint32_t)i;

    prime_count = cnt;
    prime_cap   = cnt;

    free(is_prime);
}

/* fetch the i-th prime (i >= 1) */
static uint32_t get_prime(idx_t i)
{
    if (i < 1) return 0;
    init_primes(i);
    if ((size_t)i <= prime_count)
        return prime_cache[i - 1];
    /* fallback: extend the sieve (unlikely for n <= 10000) */
    init_primes(i * 2);
    return prime_cache[i - 1];
}

seq_t a(idx_t n)
{
    /* compute primes for indices n and n+1 */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* product in wider integer to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)(p1 + 1ULL) *
                            (unsigned __int128)(p2 + 1ULL);
    prod /= 4ULL;

    return (seq_t)prod;
}
