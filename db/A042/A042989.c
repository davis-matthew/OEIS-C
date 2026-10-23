/*
 * A042989  Primes congruent to {0, 2, 3, 4, 5} mod 7.
 * https://oeis.org/A042989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes that are ≡ 0,2,3,4,5 (mod 7).  That is
   all primes except those that are 1 or 6 modulo 7.  The following
   implementation generates the required primes on demand and keeps a
   small static cache so that calls with arbitrary indices are fast
   and order‑independent. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* Static cache of primes that satisfy the congruence condition.  They
   are stored as idx_t so that we can use the native allocation
   functions without caring whether seq_t is an arbitrary–precision
   class.  The cache is expanded only when needed. */
static idx_t *prime_cache    = NULL;   /* array of primes */
static idx_t prime_cnt       = 0;      /* number of primes stored */
static idx_t prime_cap       = 0;      /* current capacity of the array */
static idx_t next_candidate  = 2;      /* candidate number to test next */

/* Helper that expands the cache in powers of two. */
static void grow_cache(idx_t needed_cap)
{
    if (prime_cap < needed_cap) {
        idx_t new_cap = prime_cap ? prime_cap << 1 : (idx_t)8;
        while (new_cap < needed_cap)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(prime_cache,
                                      (size_t)new_cap * sizeof(idx_t));
        if (!tmp) exit(1);   /* allocation failure */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the sequence */

    /* Extend the cache until it contains at least n elements. */
    while (prime_cnt < n) {
        idx_t x = next_candidate;
        ++next_candidate;            /* next number to test */

        /* Check primality.  isPrimeGeneric works for all seq_t
           representations (small or big integers). */
        if (!isPrimeGeneric((seq_t)x))
            continue;

        /* Accept only primes ≡ 0,2,3,4,5 (mod 7).  The value of
           x is guaranteed to fit in 64 bits, so the modulo can be
           computed with native integer arithmetic. */
        if (x == 7 || x % 7 == 2 || x % 7 == 3 || x % 7 == 4 || x % 7 == 5) {
            grow_cache(prime_cnt + 1);
            prime_cache[prime_cnt++] = x;
        }
    }

    /* Convert the cached idx_t prime to seq_t before returning. */
    return (seq_t)prime_cache[n - 1];
}
