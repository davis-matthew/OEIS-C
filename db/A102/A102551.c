/*
 * A102551  a(n) = [prime(n)/(prime(n+1)-prime(n))], where [x] means the
 *          integer part of x.
 * https://oeis.org/A102551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊prime(n)/(prime(n+1)-prime(n))⌋
 * Range:     u8 n <= 54, u16 n <= 6938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9633 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset = 1   (a(1) is the first listed term) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/isqrt_u64.h"

/* -------------------------------------------------------------------- */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;  /* covers first 6 primes                        */
    double nn    = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint64_t)(bound + 3.0);  /* safety margin                         */
}

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

    size_t limit = isqrt_u64(bound);
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
