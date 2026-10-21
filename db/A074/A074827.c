/*
 * A074827  Numbers n such that tau(n) > tau(n+1) where tau(x) = A000005(x).
 * https://oeis.org/A074827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that tau(m) > tau(m+1)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (tau(n) denotes the number of divisors of n) */
/* Implementation uses a dynamic sieve‑free trial division to compute
   tau(n).  Results are cached so repeated calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_simple.h"

/* Cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next integer to examine for the inequality tau(n) > tau(n+1). */
static idx_t next_candidate = 1;

/* Append a new value to the cache, growing it as necessary. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Fill the cache up to at least n terms. */
static void ensure_cache(idx_t n)
{
    idx_t m = next_candidate;
    while ((idx_t)cache_len < n) {
        uint64_t tau_m  = divisor_count_simple((uint64_t)m);
        uint64_t tau_mp = divisor_count_simple((uint64_t)(m + 1));
        if (tau_m > tau_mp)
            cache_append((seq_t)m);
        ++m;
    }
    next_candidate = m;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for index 0; return 0 as sentinel */
        return (seq_t)0;

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
