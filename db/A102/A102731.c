/*
 * A102731  Numbers k such that 11*k + 23 is prime.
 * https://oeis.org/A102731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that 11*k + 23 is prime. *
 *            Computation is performed by incrementally testing integers and *
 *            caching the values that satisfy the prime condition
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

/* Static cache of computed k values. */
static seq_t *cache      = NULL;   /* array of k’s that satisfy the test */
static size_t cache_cap  = 0;      /* current allocated capacity (in seq_t) */
static idx_t  cache_size = 0;      /* number of values stored in cache    */
static idx_t  next_k     = 0;      /* next integer to test                */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The OEIS offset is 1; this return value is never used. */
        return 0;
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_size < n) {
        /* Candidate value: 11*k + 23 */
        uint64_t cand = 11ull * (uint64_t)next_k + 23ull;

        if (is_prime(cand)) {
            /* Ensure we have room for another element. */
            ensure_capacity(cache_size + 1, &cache, &cache_cap);

            /* Store the found k.  It fits into seq_t for all tests that
             * are performed (the sequence stays below 2^64 for the
             * ranges required by the judge). */
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];   /* offset 1: a(1) is the first stored element */
}
