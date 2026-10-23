/*
 * A140551  Primes p such that neither |p - 30| nor |p + 30| is prime.
 * https://oeis.org/A140551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p such that neither |p-30| nor |p+30| is prime
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine builds the sequence on demand, memoising results in
   a static array that grows as needed.  It is thread‑unsafe but
   adequate for the intended use. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* stored terms of the sequence */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* current allocation size */

static seq_t next_candidate = 0;     /* next integer to test for primality */
static bool initialized = false;    /* true once a() has been called */

/* Append a new term to the cache, growing the allocation as needed. */
static void add_cache(seq_t val) {
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term, 1‑based. */
seq_t a(idx_t n) {
    if (!initialized) {             /* first call – initialise state */
        initialized = true;
        next_candidate = 2;          /* start with the first prime */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        if (is_prime_big(cand)) {                /* cand is prime */
            seq_t diff1 = (cand > (seq_t)30) ?
                           cand - (seq_t)30 :
                           (seq_t)30 - cand;     /* |cand-30|  */
            seq_t diff2 = cand + (seq_t)30;      /* cand+30    */

            if (!is_prime_big(diff1) && !is_prime_big(diff2))
                add_cache(cand);
        }

        /* Advance to the next candidate – skip even numbers after 2. */
        if (cand <= 2)
            next_candidate = 3;
        else
            next_candidate = cand + 2;
    }

    return cache[n - 1];                   /* a(1) is the first element */
}
