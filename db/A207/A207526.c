/*
 * A207526  Primes p such that p-2 or p+2 are semiprimes.
 * https://oeis.org/A207526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3632, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A207526: primes p such that p‑2 or p+2 is a semiprime.
   The function filters primes and tests each candidate using a
   semiprime checker.\n
   Using static cached array for memoisation.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
/* Utility functions supplied by the OEIS build */
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t *cache = NULL;   /* cache[0 … cache_size-1] hold the sequence */
static size_t cache_size = 0; /* number of filled entries */
static size_t cache_cap   = 0; /* allocated capacity of cache */
static uint64_t last_prime = 1;  /* last prime that was processed */

/* Grow the cached array to accommodate at least one more element. */
static void grow_cache(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether a 64‑bit integer is a semiprime (product of exactly two
   primes, allowing a square).  Returns true if m is a semiprime. */
static bool is_semiprime_uint64(uint64_t m)
{
    if (m < 4) return false;  /* 4 = 2*2 is the smallest semiprime */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    if (cnt == 1 && exps[0] == 2) return true;          /* perfect square of a prime */
    if (cnt == 2 && exps[0] == 1 && exps[1] == 1) return true; /* product of two distinct primes */
    return false;
}

/* Return the nth term (1‑based) of the sequence.  Index n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* invalid index, but never used */

    /* Extend the cache until we have at least n entries. */
    while (cache_size < (size_t)n) {
        /* Generate next prime in increasing order. */
        last_prime = next_prime(last_prime);

        /* Check the semiprime condition on p-2 or p+2. */
        bool ok = false;
        if (is_semiprime_uint64(last_prime - 2)) ok = true;
        else if (is_semiprime_uint64(last_prime + 2)) ok = true;

        if (ok) {
            grow_cache();
            /* Convert the uint64_t prime to seq_t and store. */
            cache[cache_size++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
