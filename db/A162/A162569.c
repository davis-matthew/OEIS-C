/*
 * A162569  Primes of the form A001043(j)-3.
 * https://oeis.org/A162569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 389, u32 n <= 46, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form prime(j) + prime(j+1) - 3 that are themselves prime */
/* The function a(n) returns the n‑th such prime, with a(1) = 2. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system. */
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

/* Cache of already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* State for generating consecutive prime pairs. */
static bool primes_initialized = false;
static uint64_t prev_prime, curr_prime;

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1, nothing defined for 0 */
        return (seq_t)0;

    /* Initialize the prime pair generator on first use. */
    if (!primes_initialized) {
        prev_prime = 2;
        curr_prime = 3;
        primes_initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t sum = (seq_t)prev_prime + (seq_t)curr_prime;
        seq_t val = sum - (seq_t)3;

        if (is_prime_big(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }

        /* Advance to the next consecutive prime pair. */
        prev_prime = curr_prime;
        curr_prime = next_prime(curr_prime);
    }

    return cache[n - 1];
}
