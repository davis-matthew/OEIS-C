/*
 * A031908  a(n) = prime(8*n - 5).
 * https://oeis.org/A031908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(8*n-5)
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cache of computed primes.  */
static uint64_t *prime_cache = NULL;
static size_t    cache_capacity = 0;
static size_t    cache_size     = 0;
static uint64_t  next_candidate = 2;

/* Return the m‑th prime (1‑based).  */
static seq_t prime_at(idx_t m)
{
    if (m == 0) {                      /* undefined, fallback to 0 */
        return (seq_t)0;
    }
    while (cache_size < (size_t)m) {
        /* Grow the cache array if needed. */
        if (cache_capacity == 0) {
            cache_capacity = 64;
            prime_cache = (uint64_t *)malloc(cache_capacity * sizeof(uint64_t));
        } else if (cache_size == cache_capacity) {
            cache_capacity <<= 1;
            prime_cache = (uint64_t *)realloc(prime_cache,
                                              cache_capacity * sizeof(uint64_t));
        }

        /* Find the next prime after the current candidate. */
        while (!is_prime(next_candidate)) {
            ++next_candidate;
        }
        prime_cache[cache_size++] = next_candidate;
        ++next_candidate;
    }

    return (seq_t)prime_cache[m - 1];
}

/* OEIS sequence A031908: a(n) = prime(8*n-5), offset 1. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;                 /* undefined for n < 1 */
    }

    idx_t m = 8 * n - 5;                  /* 1‑based index of required prime */
    return prime_at(m);
}
