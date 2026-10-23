/*
 * A126955  Numbers k such that 2*k+1, 3*k+2 and 4*k+3 are primes.
 * https://oeis.org/A126955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A126955.
   a(n) is the n‑th integer k ≥ 1 for which 2k+1, 3k+2 and 4k+3 are all prime. */
/* Dependencies: helper is_prime() from the utils package. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* The utility symbol is_prime is provided by the build, returning 1 for a prime,
   0 otherwise. It accepts an idx_t, which is typedef-ed to uint64_t. */
#include "../utils/is_prime.h"

/* Static cache of computed terms. */
static seq_t *cache = NULL;     /* array of terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Next k to test.  Starts from 0 so the first candidate is k = 1. */
static uint64_t next_k = 0;

/* Grow the cache array to hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function requested. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, guard against illegal usage */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++next_k; /* test the next candidate k */

        /* Check the three linear expressions. */
        if (is_prime(2 * next_k + 1) &&
            is_prime(3 * next_k + 2) &&
            is_prime(4 * next_k + 3)) {

            /* Store the term, expanding the cache if necessary. */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
    }

    return cache[n - 1];
}
