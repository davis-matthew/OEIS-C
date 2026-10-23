/*
 * A137944  Numbers such that the number of composite divisors is a multiple
 *          of the number of prime divisors.
 * https://oeis.org/A137944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 9440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137944
   a(n) = the nth positive integer such that the number of composite
   divisors is a multiple of the number of distinct prime divisors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prime sieve – builds a list of primes up to MAX_PRIME.  The list
    is stored in a static array and is built only once.                 */
/* --------------------------------------------------------------------- */
static const uint32_t MAX_PRIME = 2000000;         /* covers n ≤ 10000 */
static uint32_t *prime_list = NULL;               /* array of primes   */
static size_t   prime_count = 0;                  /* number of primes  */

static void build_prime_list(void)
{
    if (prime_list != NULL) return;              /* already built */

    bool *is_comp = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    if (!is_comp) return;                        /* allocation fail */

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = true;

    /* Count primes for allocation */
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) {
        free(is_comp);
        return;
    }

    /* Store primes */
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i]) prime_list[idx++] = i;

    free(is_comp);
}

/* --------------------------------------------------------------------- */
/*  Sequence function: returns the nth term of A137944.                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state: cache of found terms, next candidate, prime list */
    static seq_t *cache     = NULL;                /* terms cache    */
    static size_t   cache_cap = 0;                 /* allocated size */
    static idx_t    cache_len = 0;                 /* terms stored   */
    static uint64_t candidate = 2;                 /* start testing  */

    /* Build primes if necessary */
    if (prime_list == NULL) build_prime_list();
    if (prime_list == NULL) return 0;               /* failed to build */

    /* Allocate cache if not yet done */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        uint64_t x = candidate;
        uint32_t distinct_primes = 0;        /* Ω₀(x) */
        uint64_t tau = 1;                   /* number of total divisors */

        /* Trial division by primes */
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
            uint64_t p = prime_list[i];
            if (x % p == 0) {
                ++distinct_primes;
                uint32_t cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                tau *= (uint64_t)(cnt + 1);
            }
        }
        /* Remaining prime factor (if any) */
        if (x > 1) {
            ++distinct_primes;
            tau *= 2;
        }

        /* Composite divisor count: τ − 1 (for divisor 1) − distinct_primes */
        uint64_t composite = tau > 1 ? tau - 1 - distinct_primes : 0;
        if (distinct_primes > 0 && composite % distinct_primes == 0) {
            /* Store term */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        ++candidate;
    }

    return cache[n-1];
}
