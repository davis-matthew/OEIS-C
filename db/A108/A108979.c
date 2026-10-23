/*
 * A108979  Numbers k such that 31*k + 29 is prime.
 * https://oeis.org/A108979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer k such that 31*k + 29 is prime.
   The function caches found k values in a dynamic array of uint64_t.
   It returns the value converted to seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *k_cache = NULL;   /* dynamic array of k values (uint64_t) */
static size_t k_cache_cap = 0;     /* allocated capacity (in elements) */
static size_t k_cache_size = 0;    /* number of stored elements            */
static uint64_t next_k = 0;        /* next candidate k to test             */

/* Ensure the cache has capacity for at least new_cap elements. */
static void ensure_k_cache_size(size_t new_cap)
{
    if (k_cache_cap >= new_cap) return;
    size_t cap = k_cache_cap ? k_cache_cap : 16;
    while (cap < new_cap) cap <<= 1;            /* double until sufficient */
    uint64_t *tmp = (uint64_t*)realloc(k_cache, cap * sizeof(uint64_t));
    if (!tmp) abort();                         /* out of memory */
    k_cache = tmp;
    k_cache_cap = cap;
}

/* Return a(n) for n >= 1.  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset < 1, arbitrary */
    size_t target = (size_t)n;
    while (k_cache_size < target) {
        uint64_t candidate = next_k;
        uint64_t val = 31ULL * candidate + 29ULL;
        if (is_prime(val)) {
            ensure_k_cache_size(k_cache_size + 1);
            k_cache[k_cache_size++] = candidate;
        }
        next_k++;
    }
    return (seq_t)k_cache[target - 1];
}
