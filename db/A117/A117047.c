/*
 * A117047  Primes of the form 60*k + 11.
 * https://oeis.org/A117047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 414, u32 n <= 415, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime of the form 60*k + 11
   (a(1) = 11).  Calls are independent of order; subsequent calls
   are answered from a growing cache.  The implementation relies
   on the provided generic primality routine is_prime_big. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached primes of the form 60k+11   */
static size_t cache_len = 0;         /* number of elements in cache       */
static size_t cache_cap = 0;         /* allocated capacity of cache       */
static idx_t current_k = 0;          /* next k to test (candidate = 60*k+11) */

static void grow_cache(void)
{
    if (cache_cap < cache_len + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort to avoid undefined behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* offset is 1, so n must be >= 1 */
    if (n == 0) return 0;

    /* If we already have the value cached, return it */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Generate further primes until we reach the nth one */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)60 * current_k + (seq_t)11;
        if (is_prime_big(candidate)) {
            grow_cache();
            cache[cache_len++] = candidate;
        }
        ++current_k;
    }

    return cache[n - 1];
}
