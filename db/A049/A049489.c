/*
 * A049489  Primes p such that p + 32 is also prime.
 * https://oeis.org/A049489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 864, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth prime p such that p + 32 is also prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache of already found terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
/* Next candidate to test for primality */
static idx_t candidate = 5;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    /* Grow until we have enough space, guarding against overflow. */
    while (new_cap < need && new_cap < SIZE_MAX / 2) new_cap *= 2;
    if (new_cap < need) new_cap = need;
    /* Reallocate the cache.  Cast the result of realloc to seq_t* to
       satisfy both C and C++ compilers. */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        /* Out of memory – terminate the program (no recovery).  The
           build infrastructure treats any uncaught error as a failure. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A049489 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 to avoid underflow */

    /* Make sure we have room for the required number of terms. */
    ensure_cache_cap(n);

    /* Generate terms until the cache contains at least n values. */
    while (cache_len < n) {
        /* Find the next prime candidate. */
        while (!is_prime(candidate)) ++candidate;

        /* If p+32 is also prime, record the term. */
        if (is_prime(candidate + 32)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        /* Move to the next integer.  The outer loop will skip to the next
           prime automatically. */
        ++candidate;
    }

    return cache[n - 1];
}
