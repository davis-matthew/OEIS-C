/*
 * A033942  Positive integers with at least 3 prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A033942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033942:
   a(n) = the n-th positive integer whose total number of prime factors
   (counted with multiplicity) is at least 3.
   This implementation increments candidates one by one, using the
   provided utility count_prime_factors, and keeps a cache so that
   calls in any order remain efficient. */

/* required utility header */
#include "../utils/count_prime_factors.h"

#include <stdlib.h>
#include <stdint.h>

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0, return 0 as safety. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Static state for memoization. */
    static seq_t *cache   = NULL;  /* cached terms */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static idx_t  cache_len = 0;   /* number of terms stored */
    static seq_t  next_val = 2;    /* next candidate to test */

    /* Grow the cache when needed. */
    if (cache_len == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        /* Test next_val for Ω(next_val) >= 3. */
        if (count_prime_factors(next_val) >= (seq_t)3) {
            /* Ensure space. */
            if ((size_t)cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return (seq_t)0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        ++next_val;  /* move to next integer */
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
