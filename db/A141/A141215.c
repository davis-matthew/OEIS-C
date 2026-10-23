/*
 * A141215  Primes of the form 3*x^2+5*x*y-3*y^2 (as well as 5*x^2+9*x*y+y^2).
 * https://oeis.org/A141215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/jacobi_symbol.h"

/* A141215 – primes of the form 3*x^2+5*x*y-3*y^2
 * Sequence is the set of primes p such that either p==61
 * or the Legendre symbol (61|p)=+1.  A prime is added to the cache
 * only once (duplicates are impossible).  The routine generates primes
 * in ascending order and keeps a static cache so that calls with
 * arbitrary n are answered in O(1) after the cache is built up to n. */

static seq_t *prime_cache = NULL;      /* cache of primes in the sequence */
static size_t cache_size = 0;           /* number of cached terms */
static size_t cache_cap   = 0;           /* allocated capacity */

static idx_t next_candidate = 3;        /* next odd integer to test (2 is skipped) */

/* Ensure the cache can store at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    prime_cache = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return true if p is a prime belonging to the sequence. */
static inline bool in_sequence(idx_t p)
{
    if (p == 61U) return true;          /* the discriminant itself */
    /* For all other odd primes we require (61|p)=+1. */
    return jacobi_symbol(61U, p) == 1;
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – behaviour irrelevant */

    ensure_cache((size_t)n);

    /* Generate primes until we have `n` terms in the cache. */
    while (cache_size < (size_t)n)
    {
        idx_t p = next_candidate;
        next_candidate += 2;       /* skip even numbers (p==2 is not in the sequence) */

        if (!is_prime_small(p) && !is_prime_big((seq_t)p))
            continue;             /* composite – skip */

        if (in_sequence(p))
        {
            prime_cache[cache_size++] = (seq_t)p;
        }
    }

    return prime_cache[n - 1];
}
