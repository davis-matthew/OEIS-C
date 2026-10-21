/*
 * A239210  Numbers k such that k^2 is not divisible by any of its nonzero
 *          digits.
 * https://oeis.org/A239210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = the n‑th positive integer k such that
   k² has no non‑zero digit that divides k².  Computes terms
   on demand and caches them for future calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool passes(seq_t k)
{
    seq_t sq = k * k;
    seq_t t  = sq;
    /* Iterate over decimal digits of sq */
    while (t > 0) {
        unsigned d = (unsigned)(t % 10);
        if (d != 0 && sq % d == 0)
            return false;
        t /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;       /* number of terms stored */
    static size_t cache_cap  = 0;       /* capacity of cache array */
    static idx_t last_candidate = 0;    /* last tested integer */

    /* Ensure cache is allocated */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate further terms until we have at least n */
    while (cache_size < n) {
        ++last_candidate;
        seq_t cand = (seq_t)last_candidate;
        if (passes(cand)) {
            if (cache_size == cache_cap) {
                /* Expand cache */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure: abort */
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
