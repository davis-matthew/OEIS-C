/*
 * A178527  Primes p such that either p - 2 or p + 2 has more than two
 *          distinct prime divisors.
 * https://oeis.org/A178527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 4960, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p such that either p-2 or p+2 has more than two distinct
 * prime divisors.  Implements OEIS sequence A178527. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Requires helper from utils: */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t n); */
#include "../utils/count_distinct_primes.h" /* int count_distinct_primes(uint64_t n); */

/* Return the n-th element of sequence A178527.  n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache for terms already found. */
    static seq_t *cache    = NULL;     /* array of found terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t  next_candidate = 2;  /* next number to test for primality */

    /* Expand cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next prime number. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Inspect this prime p. */
        uint64_t p = (uint64_t)next_candidate;
        int f1 = count_distinct_primes(p - 2);
        int f2 = count_distinct_primes(p + 2);

        if (f1 > 2 || f2 > 2) {
            /* Store the term in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* In a real program we would handle allocation failure.
                       For this problem we can assert or abort. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
        /* Move to the next candidate. */
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
