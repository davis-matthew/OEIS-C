/*
 * A049408  Numbers k such that k^4 + k + 1 is prime.
 * https://oeis.org/A049408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049408: numbers k such that k^4 + k + 1 is prime.
   This implementation computes the nth such k by incremental search
   and memoisation, suitable for arbitrary‑precision, 64‑bit, or
   smaller integer types. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_k     = 0;   /* last k found */

    /* ensure at least n terms are cached */
    while (cache_len < n) {
        /* find next k with k^4 + k + 1 prime */
        for (seq_t k = last_k + 1; ; ++k) {
            seq_t k2 = k * k;
            seq_t k4 = k2 * k2;
            seq_t val = k4 + k + 1;

            if (isPrimeGeneric(val)) {
                /* store k in cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                    if (!tmp) abort();          /* allocation failure */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                last_k = k;
                break;   /* found next term */
            }
        }
    }

    return cache[(size_t)n - 1];
}
