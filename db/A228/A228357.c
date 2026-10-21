/*
 * A228357  Numbers k such that sum of all primes <= k is not prime.
 * https://oeis.org/A228357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n‑th) integer k such that sum of all primes ≤ k is
 *            composite. (OEIS A228357)
 * Range:     u8 n <= 27, u16 n <= 796, u32 n <= 828, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses a growing cache of computed terms.  It works for
   the usual 8/16/32/64‑bit unsigned integer types employed by the
   reference build, and for the arbitrary‑precision `seq_t` type used
   by the C++ build.  The only required helper functions are
   `is_prime(idx_t)` that tests primality of an idx_t, and
   `is_prime_big(seq_t)` that tests primality of a seq_t. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"        /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t) */

/* Cache of sequence values.  The cache grows as needed and is
   shared across all calls. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of stored values */

/* The largest k that has been processed so far and the running sum
   of primes ≤ that k. */
static idx_t  last_k      = 0;
static seq_t  sum_primes  = 0;

/* Append a new element to the cache, expanding it if necessary. */
static void append_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour for n < 1 is irrelevant */

    /* Ensure the cache contains at least n values. */
    while (cache_len < (size_t)n) {
        ++last_k;  /* next integer to examine */

        if (is_prime(last_k))
            sum_primes += (seq_t)last_k;

        /* If the running prime sum is composite, store the current k. */
        if (!is_prime_big(sum_primes))
            append_to_cache((seq_t)last_k);
    }

    return cache[(size_t)n - 1];
}
