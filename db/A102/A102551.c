/*
 * A102551  a(n) = [prime(n)/(prime(n+1)-prime(n))], where [x] means the
 *          integer part of x.
 * https://oeis.org/A102551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊prime(n)/(prime(n+1)-prime(n))⌋
 * Range:     u8 n <= 54, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, isqrt_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset = 1   (a(1) is the first listed term) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/isqrt_size_t.h"

/* Global cache of primes */
static uint64_t *prime_cache = NULL;    /* array of primes */
static size_t prime_cache_size = 0;     /* number of primes currently stored */
static size_t prime_cache_alloc = 0;    /* capacity of prime_cache */

/* Generate primes until we have at least 'needed' primes (1‑based). */
static void generate_primes_until(size_t needed)
{
    if (needed <= prime_cache_size)
        return;            /* already have enough */

    /* Estimate an upper bound for the needed‑th prime */
    uint64_t bound = estimate_prime_upper_bound(needed);

    /* Allocate sieve array */
    unsigned char *sieve = (unsigned char *)calloc(bound + 1, sizeof(unsigned char));
    if (!sieve)
        abort();            /* out of memory */

    sieve[0] = sieve[1] = 1;   /* mark 0 and 1 as non‑prime */

    size_t limit = isqrt_size_t(bound);
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= bound; j += i)
                sieve[j] = 1;
        }
    }

    /* Count actual primes up to the bound */
    size_t actual = 0;
    for (size_t i = 2; i <= bound; ++i)
        if (!sieve[i])
            ++actual;

    /* Allocate or grow the prime cache if needed */
    if (actual > prime_cache_alloc) {
        uint64_t *new_cache = (uint64_t *)realloc(prime_cache,
                                actual * sizeof(uint64_t));
        if (!new_cache)
            abort();
        prime_cache = new_cache;
        prime_cache_alloc = actual;
    }

    /* Fill the cache with primes, up to the requested amount */
    size_t idx = 0;
    for (size_t i = 2; i <= bound && idx < needed; ++i) {
        if (!sieve[i]) {
            prime_cache[idx++] = (uint64_t)i;
        }
    }
    prime_cache_size = idx;

    free(sieve);
}

seq_t a(idx_t n)
{
    if (n < 1)                /* defined only for n ≥ 1, guard anyway */
        return (seq_t)0;

    /* We need prime(n) and prime(n+1), so generate up to n+1 primes */
    generate_primes_until((size_t)n + 1);

    uint64_t p_n     = prime_cache[n - 1];
    uint64_t p_next  = prime_cache[n];
    uint64_t gap    = p_next - p_n;

    seq_t result = (seq_t)p_n / (seq_t)gap;   /* integer division truncates */
    return result;
}
