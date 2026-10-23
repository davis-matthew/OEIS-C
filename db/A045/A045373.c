/*
 * A045373  Primes congruent to {0, 1, 2, 4} mod 7.
 * https://oeis.org/A045373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045373: a(n) = the n‑th prime p with p ≡ 0,1,2,4 (mod 7) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* Return the n‑th prime congruent to 0,1,2 or 4 modulo 7.  */
seq_t a(idx_t n)
{
    /* cache of computed sequence values */
    static seq_t *cache  = NULL;
    static size_t cacheSize = 0;
    static size_t cacheCap  = 0;

    /* value to test next */
    static uint64_t current = 2;

    /* already computed? */
    if (cache && n <= cacheSize)
        return cache[n - 1];

    /* initialise cache */
    if (!cacheCap) {
        cacheCap = 16;
        cache = (seq_t*)malloc(cacheCap * sizeof(seq_t));
    }

    /* generate terms until we have n of them */
    while (cacheSize < n) {
        uint64_t cand = current++;
        uint64_t r = cand % 7ULL;
        if (r == 0 || r == 1 || r == 2 || r == 4) {
            if (isPrimeGeneric((seq_t)cand)) {
                /* grow cache if necessary */
                if (cacheSize == cacheCap) {
                    cacheCap *= 2;
                    cache = (seq_t*)realloc(cache, cacheCap * sizeof(seq_t));
                }
                cache[cacheSize++] = (seq_t)cand;
            }
        }
    }

    return cache[n - 1];
}
