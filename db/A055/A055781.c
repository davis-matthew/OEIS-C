/*
 * A055781  Primes q of the form q = 10p + 1, where p is also prime.
 * https://oeis.org/A055781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime q such that q = 10p+1 for some prime p
 * Range:     u8 n <= 4, u16 n <= 154, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055781: primes q of the form 10p+1 where p itself is prime.
   Implementation uses a simple incremental search over primes p and
   stores the resulting q values in a growable cache. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* is_prime(idx_t) for 64‑bit primes */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t) for arbitrary size */

static seq_t *cache = NULL;   /* array of found q values */
static idx_t cache_len = 0;   /* number of elements stored */
static idx_t cache_cap = 0;   /* allocated capacity */
static uint64_t last_p = 1;   /* last prime p examined */

/* Ensure cache has capacity for at least 'needed' elements.  */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (size_t)cache_cap : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    /* realloc returns void*; cast explicitly for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* abort if memory allocation fails */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Return the nth term of A055781 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; 0 is unused */

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        /* Find next prime p greater than last_p. */
        uint64_t p = last_p + 1;
        while (!is_prime((idx_t)p)) ++p;
        last_p = p;

        /* Candidate q = 10p + 1. */
        seq_t q = (seq_t)10 * (seq_t)p + 1;

        /* Keep q if it is also prime. */
        if (is_prime_big(q)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = q;
        }
    }

    return cache[n - 1];
}
