/*
 * A140560  Primes p such that p + 30 is not a prime.
 * https://oeis.org/A140560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that p+30 is not prime
 * Range:     u8 n <= 17, u16 n <= 4188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes in ascending order, tests
   whether p+30 is prime, and stores the qualifying primes in a
   static cache so that any call to a(idx_t n) returns the n‑th term
   correctly for all n ≥ 1, irrespective of the order of calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;        /* cache of sequence values          */
static size_t cache_cap = 0;        /* current cache capacity            */
static size_t cache_size = 0;       /* number of values stored           */
static idx_t next_checked = 2;      /* next integer to test for primality */

/* Extend the cache when necessary.  The cast is required for C++,
   where realloc returns void*.  It is safe for both C and C++ builds. */
static void grow_cache(void) {
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence.  n is 1‑based.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for 0, return zero as a guard */

    while (cache_size < n) {
        if (is_prime(next_checked)) {
            if (!is_prime(next_checked + 30)) {
                grow_cache();
                cache[cache_size++] = (seq_t)next_checked;
            }
        }
        ++next_checked;
    }
    return cache[n - 1];
}
