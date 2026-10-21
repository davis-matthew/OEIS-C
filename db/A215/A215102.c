/*
 * A215102  Primes congruent to {2, 3, 5} mod 11.
 * https://oeis.org/A215102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes congruent to {2,3,5} modulo 11
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental search with a static cache. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed, seq_t **cache_ptr, size_t *cap_ptr)
{
    if (needed > *cap_ptr) {
        size_t newcap = *cap_ptr ? (*cap_ptr) * 2 : 128;
        while (needed > newcap) newcap <<= 1;
        seq_t *newcache = (seq_t *)realloc(*cache_ptr, newcap * sizeof(seq_t));
        *cache_ptr = newcache;
        *cap_ptr = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* static state for lazy generation */
    static seq_t *cache = NULL;
    static size_t size = 0;        /* number of primes stored */
    static size_t cap  = 0;        /* allocated capacity */
    static seq_t cur   = (seq_t)2; /* next number to test */

    /* initialize array on first call */
    if (cache == NULL) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        size  = 0;
    }

    /* generate primes until we have at least n of them */
    while (size < (size_t)n) {
        seq_t cand = cur++;            /* candidate number */
        seq_t rem  = cand % (seq_t)11; /* test residues */

        if (rem != 2 && rem != 3 && rem != 5) continue;

        /* primality test (works for any seq_t size) */
        if (!is_prime_big(cand)) continue;

        ensure_capacity(size + 1, &cache, &cap);
        cache[size++] = cand;
    }

    return cache[n - 1];
}
