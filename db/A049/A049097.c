/*
 * A049097  Primes p such that p+1 is squarefree.
 * https://oeis.org/A049097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049097:  Primes p such that p+1 is square‑free.
   a(1) = 2,  a(2) = 5,  a(3) = 13, ...
   The implementation builds primes on demand and caches all values
   that satisfy the square‑free condition.  It works for any
   sequence element type seq_t (unsigned integer or arbitrary‑precision
   class) and any index type idx_t (uint64_t).  Only trivial memory
   allocations (realloc) are used, which are safe in both C11 and
   C++17.  The function is thread‑aborted on allocation failure.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Prime list used for trial division and for factoring.  */
static uint64_t *primes = NULL;
static size_t   primes_size = 0;
static size_t   primes_cap  = 0;

/* Cache of primes p with p+1 square‑free.  */
static seq_t *cache = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap  = 0;

/* Next natural number to be examined for primality.  */
static uint64_t next_candidate = 2;

/* Ensure 'primes' has room for at least 'needed' elements. */
static void ensure_primes_cap(size_t needed)
{
    if (needed <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    void *tmp = realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                 /* allocation failure  */
    primes = (uint64_t *)tmp;
    primes_cap = new_cap;
}

/* Ensure 'cache' has room for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
}

/* Test whether a 64‑bit number m is square‑free using the primes list. */
static bool squarefree_uint64(uint64_t m)
{
    if (m <= 1)
        return true;

    for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            m /= p;
            if (m % p == 0)   /* exponent > 1 */
                return false;
        }
    }
    return true;
}

/* Simple primality test based on the primes list. */
static bool is_candidate_prime(uint64_t cand)
{
    if (cand < 2)
        return false;
    for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= cand; ++i) {
        if (cand % primes[i] == 0)
            return false;
    }
    return true;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined below the offset */

    /* Grow the cache if necessary. */
    if (n > cache_cap)
        ensure_cache_cap((size_t)n);

    /* Generate terms until we have at least n entries. */
    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate++;

        /* Find the next prime candidate. */
        while (!is_candidate_prime(cand)) {
            cand = next_candidate++;
        }

        /* Store prime for future trial division checks. */
        ensure_primes_cap(primes_size + 1);
        primes[primes_size++] = cand;

        /* Check the square‑free property of cand + 1. */
        if (squarefree_uint64(cand + 1)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[(size_t)n - 1];
}
