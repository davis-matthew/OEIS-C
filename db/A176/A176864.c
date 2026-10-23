/*
 * A176864  Numbers k such that A053186(k) is prime.
 * https://oeis.org/A176864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k = p + j^2 where p is prime and p <= 2*j. */

/* The function builds the sequence lazily, storing all previously
 * computed terms in a static array.  For each new segment of the
 * sequence we increment j, iterate all primes p <= 2*j,
 * compute k = p + j^2, and append the result in sorted order
 * (the intervals for increasing j are disjoint). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* provides is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t cur_j = 0;           /* last j processed */

    /* Grow cache capacity if needed */
    if (cache_cap == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        cur_j++;                     /* next j */

        /* For each prime p <= 2*j, generate k = p + j^2 */
        idx_t limit = 2 * cur_j;
        for (idx_t p = 2; p <= limit; p++) {
            if (!is_prime(p))
                continue;          /* skip non-primes */

            /* Compute k = p + j^2; use seq_t for all ops */
            seq_t j_sq = (seq_t)cur_j * (seq_t)cur_j;
            seq_t k     = (seq_t)p + j_sq;

            /* Append k to cache */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;
        }
    }

    /* Return the requested term (sequence is 1‑based) */
    return cache[n - 1];
}
