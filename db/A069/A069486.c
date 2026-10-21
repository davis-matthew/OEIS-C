/*
 * A069486  a(n) = 2*prime(n)*prime(n+1).
 * https://oeis.org/A069486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n)*prime(n+1)
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple incremental prime generator with caching.
   It is independent of the build‑provided utilities. */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_count = 0;       /* number of primes stored */
static size_t   prime_cap   = 0;       /* allocated capacity */

/* Ensure the cache holds at least needed primes (1‑based index). */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Grow capacity if necessary. */
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort(); /* out of memory */
        prime_cache = tmp;
        prime_cap = new_cap;
    }

    /* Start generating from the next candidate. */
    uint64_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;
    while (prime_count < needed) {
        bool is_prime = true;
        /* Test divisibility by known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_cache[prime_count++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, but harmless */

    /* Ensure we have primes up to index n+1 (1‑based). */
    ensure_primes((size_t)(n + 1));

    seq_t p1 = (seq_t)prime_cache[(size_t)n - 1];       /* prime(n)   */
    seq_t p2 = (seq_t)prime_cache[(size_t)n];           /* prime(n+1) */
    return (seq_t)2 * p1 * p2;                          /* 2*prime(n)*prime(n+1) */
}
