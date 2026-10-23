/*
 * A046340  Odd composite numbers with an odd number of prime factors (counted
 *          with multiplicity).
 * https://oeis.org/A046340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 9974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A046340 – odd composite numbers with an odd number of prime factors
   (counted with multiplicity).  The function generates the sequence by
   testing successive odd numbers, skipping primes, and keeping only those
   composite odd numbers whose total number of prime factors (with multiplicity)
   is odd.  A static cache is used so that calls with arbitrary indices are
   answered efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers that provide the required helper functions. */
#include "../utils/is_prime.h"
#include "../utils/bigomega.h"

/* Static cache of the sequence values. */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of values currently stored */
static size_t cache_cap = 0;     /* current capacity of the cache */
static uint64_t last_candidate = 1;   /* last odd number tested */

/* Ensure that at least 'n' terms are present in the cache.
   n is 1‑based; cache indices are 0‑based. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Advance to the next odd number. */
        last_candidate += 2;
        if (last_candidate == 1)          /* skip 1 */
            continue;

        /* Skip prime numbers – we only want composites. */
        if (is_prime(last_candidate))
            continue;

        /* Count prime factors with multiplicity using the provided helper. */
        seq_t omega = bigomega((idx_t)last_candidate);

        /* Store the number if the count is odd. */
        if ((omega & 1) == 1) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real library we would handle this error. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }
}

/* Return the nth term of the sequence (1‑based).  For n <= 0 a value of 0
   is returned, which is harmless because the sequence offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n - 1];
}
