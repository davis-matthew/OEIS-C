/*
 * A066941  Numbers k such that gcd(prime(k+1) + 1, prime(k) + 1) = 4.
 * https://oeis.org/A066941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function finds the n‑th integer k such that
   gcd(prime(k)+1 , prime(k+1)+1) == 4.
   It scans successive k, caching the results so that
   subsequent calls (in any order) are fast. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

/* Cache of the first terms that satisfy the condition. */
static seq_t *cache   = NULL;   /* array of k values */
static size_t cache_len = 0;    /* how many terms are stored */
static size_t cache_cap = 0;    /* allocated array capacity */
static idx_t last_k_tested = 0; /* the largest k that has been checked */

/* Grow the cache array when it is full. */
static void grow_cache(void) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* not defined for offset 1, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_k_tested;

        /* Get primes p_k and p_{k+1}. */
        uint32_t p_k  = get_prime(last_k_tested);
        uint32_t p_k1 = get_prime(last_k_tested + 1);

        /* Compute gcd of (prime+1) values. */
        seq_t v1 = (seq_t)p_k  + 1;
        seq_t v2 = (seq_t)p_k1 + 1;
        seq_t g  = gcd(v1, v2);

        /* If the condition holds, store k in the cache. */
        if (g == (seq_t)4) {
            grow_cache();
            cache[cache_len++] = (seq_t)last_k_tested;
        }
    }

    return cache[n - 1];
}
