/*
 * A028979  Palindromes which are the product of two consecutive primes.
 * https://oeis.org/A028979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A028979: palindromes which are the product
   of two consecutive primes.
   The function builds the sequence incrementally, storing all
   generated palindromes in ascending order.  It uses no predetermined
   bounds and is memoised so that calls with arbitrary indices are
   correct and efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t n); */
#include "../utils/is_palindrome.h" /* bool is_palindrome(seq_t x, idx_t base); */

/* compute the maximum value that can be represented by type seq_t */
static unsigned __int128 seq_t_max(void) {
    unsigned __int128 m = ((unsigned __int128)1 << (sizeof(seq_t) * 8)) - 1;
    return m;
}

seq_t a(idx_t n) {
    static seq_t *cache      = NULL;   /* sorted palindromic products */
    static size_t  cache_sz  = 0;      /* number of stored values */
    static size_t  cache_cap = 0;      /* allocated capacity */

    static uint64_t *primes     = NULL; /* list of primes discovered so far */
    static size_t   primes_cnt  = 0;    /* number of primes stored */
    static size_t   primes_cap  = 0;    /* allocated capacity for primes */

    static uint64_t next_candidate = 2; /* next number to test for primality */

    /* initialise static storage on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz = 0;

        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
    }

    /* repeatedly generate consecutive primes and check their product */
    unsigned __int128 max_val = seq_t_max();
    while (cache_sz < n) {
        /* find the next prime */
        uint64_t p = next_candidate;
        while (!is_prime(p))
            ++p;
        next_candidate = p + 1; /* prepare start for next search */

        /* store the new prime */
        if (primes_cnt == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_cnt++] = p;

        /* if we have at least two primes, compute the product of the last
           two consecutive primes and check if it is a palindrome */
        if (primes_cnt >= 2) {
            uint64_t q = p;
            uint64_t r = primes[primes_cnt - 2];
            unsigned __int128 prod128 = (unsigned __int128)r * (unsigned __int128)q;

            /* skip if product exceeds the representable range of seq_t */
            if (prod128 > max_val)
                continue;

            seq_t prod = (seq_t)prod128;
            if (is_palindrome(prod, 10)) {
                if (cache_sz == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_sz++] = prod;
            }
        }
    }

    return cache[n - 1];
}
