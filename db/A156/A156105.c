/*
 * A156105  Primes p such that p + 72 is also prime.
 * https://oeis.org/A156105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p such that p‑72 is also prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches primes on demand and caches the found
   terms. It uses the supplied is_prime_big function for primality testing. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Cache for sequence terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test for being a qualifying prime */
static uint64_t next_candidate = 2;

/* Ensure the cache contains at least 'need' terms */
static void ensure_terms(idx_t need)
{
    /* Expand the cache in powers of two */
    if (cache_cap < (size_t)need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Search for further qualifying primes until we have 'need' terms */
    while (cache_len < (size_t)need) {
        seq_t cand = (seq_t)next_candidate;         /* current candidate   */
        seq_t cand_plus = cand + 72;                /* p + 72              */

        /* Is cand a prime and is cand_plus a prime? */
        if (is_prime_big(cand) && is_prime_big(cand_plus)) {
            cache[cache_len++] = cand;              /* store it           */
        }

        ++next_candidate;  /* advance for the next trial              */
    }
}

/* Return the n-th term of the sequence (a(1) = 7) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */
    ensure_terms(n);
    return cache[n - 1];
}
