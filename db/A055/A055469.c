/*
 * A055469  Primes of the form k(k+1)/2+1 (i.e., central polygonal numbers, or
 *          one more than triangular numbers).
 * https://oeis.org/A055469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form k(k+1)/2 + 1 (k = 1, 2, …)
 * Range:     u8 n <= 10, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a dynamic array of found terms.
   For each new term it tests successive k values.  Primality is
   checked with the utility function is_prime which operates on
   64‑bit integers.  As the sequence grows slowly, 64‑bit arithmetic
   suffices for the first 10 000 terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;  /* last k that was tested */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_k = 0;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* next k to test */
        ++last_k;

        /* compute the candidate value: k(k+1)/2 + 1 */
        uint64_t cand64 = (uint64_t)last_k * ((uint64_t)last_k + 1U) / 2U + 1U;

        /* test primality */
        if (is_prime(cand64)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* store the prime as a seq_t */
            cache[cache_len++] = (seq_t)cand64;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
