/*
 * A126711  Primes of the form p*q*r*s+2 with p,q,r,s odd primes.
 * https://oeis.org/A126711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime p such that p-2 has exactly 4 prime
 *            factors (counted with multiplicity)
 * Range:     u8 n <= 4, u16 n <= 1052, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const size_t MAX_LIMIT = 12000000;          /* Upper bound for sieve (≈12 million). */
static uint32_t *spf = NULL;                      /* Smallest prime factor array. */
static uint32_t *primes = NULL;                   /* Array of primes up to MAX_LIMIT. */
static size_t primes_cnt = 0;                     /* Number of primes stored. */
static size_t primes_cap = 0;                     /* Capacity of primes array. */

static seq_t *cache = NULL;                       /* Cached sequence values. */
static idx_t cache_size = 0;                      /* Number of cached values. */
static size_t cache_cap = 0;                      /* Capacity of cache array. */

static size_t prime_idx = 0;                      /* Current index in primes array for generation. */

/* Helper: Initialize sieve, spf array, and primes list. */
static void init_sieve(void)
{
    if (spf != NULL)
        return;   /* Already initialized. */

    /* Allocate spf array init with 0. */
    spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (spf == NULL) {
        abort(); /* Memory exhaustion. */
    }

    /* Allocate a temporary bit array for composites. */
    bool *composite = (bool *)calloc(MAX_LIMIT + 1, sizeof(bool));
    if (composite == NULL) {
        abort();
    }

    /* Allocate primes array with generous capacity. */
    primes_cap = MAX_LIMIT / 10 + 10;  /* Rough estimate. */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (primes == NULL) {
        abort();
    }

    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!composite[i]) {
            /* i is prime. */
            if (primes_cnt == primes_cap) {
                /* Expand primes array. */
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
            spf[i] = (uint32_t)i;

            if (i * i <= MAX_LIMIT) {
                for (size_t j = i * i; j <= MAX_LIMIT; j += i) {
                    composite[j] = true;
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }

    free(composite);
}

/* Compute bigomega(n) for n <= MAX_LIMIT using spf array. */
static unsigned bigomega_uint32(uint32_t n)
{
    unsigned cnt = 0;
    while (n > 1) {
        ++cnt;
        n /= spf[n];
    }
    return cnt;
}

/* Extend cache until it contains at least target terms. */
static void ensure_cache(idx_t target)
{
    if (target <= cache_size)
        return;

    if (spf == NULL)
        init_sieve();

    /* Expand cache array if necessary. */
    while (cache_size < target) {
        if (prime_idx >= primes_cnt) {
            /* Should not happen for target <= 10 000 with chosen LIMIT. */
            abort();
        }
        uint32_t p = primes[prime_idx++];
        if (bigomega_uint32(p - 2) == 4) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* Not used for OEIS offset 1. */
    ensure_cache(n);
    return cache[n - 1];
}
