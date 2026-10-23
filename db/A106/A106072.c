/*
 * A106072  Primes p such that 3*p + 16 and 16*p + 3 are primes.
 * https://oeis.org/A106072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 3*p+16 and 16*p+3 are prime,
 *            listed in increasing order (offset 1). The sequence is generated
 *            by a greedy scan over primes, only storing the values that
 *            satisfy the condition
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isPrimeMR64, nextPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeMR64.h"
#include "../utils/nextPrime.h"

/* Cache for the sequence.  Stored from a(1) at index 0. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* The required sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* Undefined for offset 1; safe fallback. */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Allocate or grow cache. */
    if (cache_len == 0 || cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate primes until we have the nth term. */
    while (cache_len < n) {
        uint32_t p = nextPrime();            /* p is a prime. */
        uint64_t q1 = 3ULL * p + 16ULL;      /* 3p + 16 */
        uint64_t q2 = 16ULL * (uint64_t)p + 3ULL;  /* 16p + 3 */

        if (isPrimeMR64(q1) && isPrimeMR64(q2)) {
            /* Store the qualifying prime. */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
