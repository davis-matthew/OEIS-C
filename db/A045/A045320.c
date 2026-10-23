/*
 * A045320  Primes not congruent to 5 (mod 7).
 * https://oeis.org/A045320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 5449, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes not congruent to 5 modulo 7
 * (OEIS A045320).  The n‑th term is the n‑th prime p such that p % 7
 * != 5.  The implementation maintains a growing cache of qualifying
 * primes and expands it lazily whenever a larger index is requested.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;   /* cache of qualifying primes */
static size_t   cache_cap   = 0;       /* allocated capacity */
static size_t   cache_sz    = 0;       /* number of primes stored */
static uint64_t next_candidate = 1;    /* next integer to test */

/* Grow the cache array so that it can hold at least `need` elements. */
static void
ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    /* In C++ realloc returns void*, so cast explicitly. */
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) {
        /* Allocation failure – terminate the program. */
        exit(EXIT_FAILURE);
    }
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Extend the cache until it contains at least `n+1` qualifying primes
 * (zero‑based index). */
static void
grow_cache_to(idx_t n)
{
    while (cache_sz <= (size_t)n) {
        /* Test the candidate. */
        if (is_prime(next_candidate) &&
            (next_candidate % 7ULL) != 5ULL) {
            ensure_capacity(cache_sz + 1);
            prime_cache[cache_sz++] = next_candidate;
        }
        /* Advance to the next candidate: test all integers, but skip
         * even numbers after 2. */
        if (next_candidate < 2)
            next_candidate = 2;
        else if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* Return the n-th term of the sequence (a(1) = 2). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1 */

    size_t idx = (size_t)n - 1;      /* zero‑based cache index */
    grow_cache_to((idx_t)idx);
    return (seq_t)prime_cache[idx];
}
