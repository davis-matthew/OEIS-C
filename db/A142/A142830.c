/*
 * A142830  Primes congruent to 32 mod 61.
 * https://oeis.org/A142830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 32 (mod 61)
 * Range:     u8 none, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by sieving up to a growing limit. */

#include <stdlib.h>
#include <stdint.h>

/* Static state ---------------------------------------------------------- */
static idx_t      sieve_limit      = 0;   /* highest number sieved so far   */
static uint8_t   *is_composite     = NULL;/* 0 = prime, 1 = composite       */
static uint64_t  *prime_cache      = NULL;/* primes ≡ 32 (mod 61) found      */
static size_t    prime_cap        = 0;   /* allocated capacity of cache    */
static size_t    prime_cnt        = 0;   /* number of elements in cache    */

/* Append a prime to the cache, resizing if necessary */
static void cache_append(uint64_t p)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        uint64_t *tmp = (uint64_t*)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* memory exhausted */
        prime_cache = tmp;
        prime_cap = new_cap;
    }
    prime_cache[prime_cnt++] = p;
}

/* Sieve of Eratosthenes up to new_limit and rebuild the prime cache   */
static void expand_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;                      /* already covered */

    /* Allocate new sieve array */
    uint8_t *new_sieve = (uint8_t*)calloc((size_t)(new_limit + 1), 1);
    if (!new_sieve) abort();         /* memory exhausted */

    /* Run sieve of Eratosthenes on 0…new_limit */
    idx_t lim = new_limit;
    for (idx_t i = 2; (i * i) <= lim; ++i) {
        if (!new_sieve[i]) {
            for (idx_t j = i * i; j <= lim; j += i)
                new_sieve[j] = 1;
        }
    }

    /* Free old data */
    free(is_composite);
    free(prime_cache);
    is_composite = new_sieve;
    prime_cache  = NULL;
    prime_cap    = 0;
    prime_cnt    = 0;

    /* Populate cache with primes ≡ 32 (mod 61) */
    for (idx_t n = 2; n <= lim; ++n) {
        if (!is_composite[n] && ((n - 32U) % 61U) == 0U)
            cache_append((uint64_t)n);
    }

    sieve_limit = new_limit;
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined, harmless */

    /* Ensure we have found at least n desired primes */
    size_t need = (size_t)n;
    if (prime_cnt < need) {
        idx_t limit = sieve_limit ? sieve_limit << 1 : 2048;
        while (prime_cnt < need) {
            expand_sieve(limit);
            limit <<= 1;              /* double for next iteration */
        }
    }

    return (seq_t)prime_cache[n - 1];
}
