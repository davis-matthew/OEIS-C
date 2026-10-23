/*
 * A142308  Primes congruent to 37 mod 44.
 * https://oeis.org/A142308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142308: primes congruent to 37 mod 44 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* The function returns the nth prime p satisfying p ≡ 37 (mod 44),
   with a(1) = 37.  The result is stored in seq_t; it is assumed
   that is_prime(idx_t) is provided by the framework. */

/* Static state for memoization. */
static idx_t     next_k   = 0;     /* next k to produce candidate 44*k+37 */
static idx_t     cache_cnt = 0;    /* number of cached terms */
static size_t    cache_cap = 0;    /* allocated size of cache */
static seq_t *   cache     = NULL; /* array of cached terms */

/* Ensure the cache can hold at least n elements. */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, but safe */

    ensure_cache_size(n);

    while (cache_cnt < n) {
        /* Candidate of the form 44*k + 37 */
        idx_t candidate = 44 * next_k + 37;
        ++next_k;
        if (is_prime(candidate)) {
            cache[cache_cnt] = (seq_t)candidate;
            ++cache_cnt;
        }
    }

    return cache[n - 1];
}
