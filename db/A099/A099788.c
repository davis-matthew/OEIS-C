/*
 * A099788  a(n) = Product_{i=1..2n} prime(i).
 * https://oeis.org/A099788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{i=1}^{2n} prime(i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates the nth term of the sequence A099788.
 * For any n >= 0 it returns the product of the first 2*n primes.
 * The result is computed incrementally and cached so that repeated
 * calls are efficient.  A simple sieve of Eratosthenes is used to
 * generate as many primes as needed.  The implementation handles
 * both built‑in integral types and arbitrary‑precision integer
 * types automatically.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *global_primes = NULL;    /* array of generated primes      */
static size_t primes_cnt = 0;              /* number of primes stored         */
static size_t sieve_limit = 0;             /* maximum value sieved so far      */

static seq_t *cache = NULL;                /* cached sequence values           */
static size_t cache_len = 0;                /* number of cached terms           */
static size_t cache_cap = 0;                /* capacity of cache array          */

/* -------------------------------------------------------------
   Prime sieve: rebuilds the prime table up to 'limit'.
   ------------------------------------------------------------- */
static void build_primes_upto_limit(size_t limit)
{
    if (limit <= sieve_limit) return;

    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;          /* allocation failure – give up */

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    free(global_primes);
    /* Allocate a generous amount – the actual number of primes is less. */
    global_primes = (uint32_t *)malloc(limit * sizeof(uint32_t));
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) global_primes[primes_cnt++] = (uint32_t)i;
    }
    free(is_prime);
    sieve_limit = limit;
}

/* -------------------------------------------------------------
   Ensure at least 'needed' primes are available.
   ------------------------------------------------------------- */
static void ensure_primes(size_t needed)
{
    if (primes_cnt >= needed) return;
    /* start from a modest limit and double until we have enough primes */
    size_t limit = sieve_limit ? sieve_limit : 100000;
    if (limit == 0) limit = 100000;
    while (primes_cnt < needed) {
        limit *= 2;
        if (limit < 1000) limit = 1000;
        build_primes_upto_limit(limit);
    }
}

/* -------------------------------------------------------------
   Seq_t a(idx_t n) – main function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache for a(0) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If we already have the value, return it */
    if (cache_len > (size_t)n) return cache[n];

    /* Need more terms */
    ensure_primes((size_t)2 * n);

    seq_t prev_prod = cache[cache_len - 1];
    for (idx_t k = cache_len; k <= n; ++k) {
        /* multiply by the primes with indices 2k-2 and 2k-1 (0‑based) */
        prev_prod *= (seq_t)global_primes[(size_t)2 * k - 2];
        prev_prod *= (seq_t)global_primes[(size_t)2 * k - 1];

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap * 2 + 1;      /* avoid zero */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = prev_prod;
    }

    return cache[n];
}
