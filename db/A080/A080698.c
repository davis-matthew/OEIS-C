/*
 * A080698  Product of twin-prime-indexed primes and their lower bound twin
 *          prime.
 * https://oeis.org/A080698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 336, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_limit_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A080698
 *
 *  a(n) = prime( twin_lower(n) ) * twin_lower(n)
 *  where twin_lower(n) is the lower member of the nth pair of twin primes.
 *
 *  The function uses on‑demand sieving and caching of prime numbers and
 *  twin prime lower bounds.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_limit_for_k.h"

/* ------------------------------------------------------------------ */
/* Global (static) data structures – kept between calls                */
static uint32_t *primes       = NULL;   /* primes[i] = i‑th prime (1‑indexed)      */
static size_t   prime_count   = 0;       /* number of primes stored                */
static uint32_t *twin_lower   = NULL;   /* lower member of twin prime pairs          */
static size_t   twin_count    = 0;       /* number of twin pairs stored             */
static idx_t    sieve_limit   = 0;       /* current sieve upper bound               */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Recompute all primes and twin prime lower bounds up to 'limit'.
 * Old data is freed and replaced.                                         */
static void recompute_up_to(idx_t limit)
{
    free(primes);
    free(twin_lower);
    primes      = NULL;
    twin_lower  = NULL;
    prime_count = 0;
    twin_count  = 0;

    /* Allocate primality bitset */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    idx_t sqrt_lim = (idx_t)sqrt((double)limit) + 1;
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Allocate dynamic arrays for primes and twin lowers   */
    size_t prime_cap = 8;
    size_t twin_cap  = 8;
    primes      = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    twin_lower  = (uint32_t *)malloc(twin_cap * sizeof(uint32_t));
    if (!primes || !twin_lower) exit(EXIT_FAILURE);

    uint32_t prev_prime = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i]) continue;

        /* store prime */
        if (prime_count == prime_cap) {
            prime_cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes = tmp;
        }
        primes[prime_count++] = (uint32_t)i;

        /* check for twin with previous prime */
        if (prev_prime && (i - prev_prime == 2)) {
            if (twin_count == twin_cap) {
                twin_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(twin_lower, twin_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                twin_lower = tmp;
            }
            twin_lower[twin_count++] = prev_prime;
        }
        prev_prime = (uint32_t)i;
    }

    free(is_prime);
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that all data required for computing a(n) are available.    */
static void ensure_sequence_to(idx_t n)
{
    if (n < 1) return;

    /* (1)  Make sure we have at least n twin prime pairs.           */
    if (twin_count < (size_t)n) {
        idx_t limit = sieve_limit ? sieve_limit : 100000;
        while (twin_count < (size_t)n) {
            recompute_up_to(limit);
            if (twin_count >= (size_t)n) break;
            limit <<= 1;                      /* double the limit */
        }
    }

    /* (2)  Determine the index p = twin_lower(n) and ensure we have
          at least p primes.                                        */
    uint32_t p_needed = twin_lower[n - 1];
    if (prime_count < (size_t)p_needed) {
        idx_t limit = sieve_limit ? sieve_limit : 100000;
        while (prime_count < (size_t)p_needed) {
            idx_t est = estimate_limit_for_k((idx_t)p_needed);
            if (est > limit) limit = est;
            else limit <<= 1;
            recompute_up_to(limit);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function: a(n) = prime(twin_lower(n)) * twin_lower(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */
    ensure_sequence_to(n);

    uint32_t p = twin_lower[n - 1];          /* lower member of nth twin pair */
    seq_t prime_of_p = (seq_t)primes[p - 1]; /* primes array is 1‑indexed   */
    return (seq_t)p * prime_of_p;
}
