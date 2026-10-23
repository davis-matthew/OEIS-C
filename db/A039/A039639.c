/*
 * A039639  Fixed point of "k -> k/2 or (k-1)/2 until result is prime",
 *          starting with prime(n)+1.
 * https://oeis.org/A039639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 7539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the map k -> k/2 or (k-1)/2 until the result is prime,
   starting with prime(n)+1.  For each n ≥ 1, a(n) is the first prime
   reached by that iteration. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions (provided by the OEIS build system).  Their
   corresponding headers are located in ../utils/.  Include only what
   we need here. */
#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **out, size_t *cnt) */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) */

/* Cache of the first N primes.  It is extended lazily on demand. */
static idx_t *prime_list = NULL;      /* pointer to array of primes */
static size_t prime_count = 0;        /* number of primes currently stored */
static idx_t  prime_limit = 0;        /* current upper bound used for the sieve */

/* Ensure that the prime cache contains at least the first 'n' primes. */
static void
ensure_primes(idx_t n)
{
    /* If we already have enough primes, nothing to do. */
    if (prime_count >= (size_t)n)
        return;

    /* Determine a new sieve limit.  Start with a modest bound and
       double it until we have at least n primes. */
    if (prime_limit == 0) {
        /* For small n use a small fixed limit; otherwise start with n*15
           which is a safe over‑estimate for the n‑th prime up to
           millions. */
        prime_limit = (n < 6) ? 15U : n * 15U;
    } else {
        /* Grow exponentially to keep the number of re‑sieves small. */
        prime_limit *= 2;
    }

    /* Rebuild the sieve with the new limit. */
    idx_t *tmp;            /* temporary pointer for the new list */
    size_t tmp_cnt;        /* number of primes found */

    build_primes(prime_limit, &tmp, &tmp_cnt);

    /* Replace the old list (if any). */
    free(prime_list);               /* free old list (may be NULL) */
    prime_list = tmp;
    prime_count = tmp_cnt;

    /* If we still do not have enough primes, repeat the process. */
    if (prime_count < (size_t)n)
        ensure_primes(n);
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n = 0 – guard against UB */

    /* Make sure we have at least n primes in the cache. */
    ensure_primes(n);

    /* Start from prime(n)+1.  prime_list is zero‑based. */
    seq_t k = (seq_t)prime_list[ (size_t)n - 1 ] + (seq_t)1;

    /* Iterate the map until a prime is reached. */
    while (!is_prime_big(k)) {
        if ((k & (seq_t)1) == 0)      /* k even  -> k/2 */
            k >>= 1;
        else                          /* k odd   -> (k-1)/2 */
            k = (k - (seq_t)1) >> 1;
    }

    return k;
}
