/*
 * A143758  Aliquot sequence starting at 84.
 * https://oeis.org/A143758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Aliquot sequence starting at 84:
   a(0) = 84
   a(n+1) = A001065(a(n))  (sum of proper divisors of a(n)) */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sigma_of.h"

extern seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored a(i) values */
    static size_t size = 0;       /* number of values stored */
    static size_t cap  = 0;       /* capacity of cache */

    /* initialize with first term */
    if (size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)84;
        size = 1;
        cap  = 1;
    }

    /* if requested index already computed, just return it */
    if (n < size) {
        return cache[n];
    }

    /* grow the cache up to at least n+1 elements */
    if (n >= cap) {
        size_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* memory allocation failed: return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute subsequent terms */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        if (prev == (seq_t)0) {
            cache[i] = (seq_t)0;
        } else {
            idx_t idx_prev = (idx_t)prev;                      /* prev fits in 64‑bit */
            seq_t sigma    = sigma_of(idx_prev);               /* σ(prev) */
            cache[i] = sigma - prev;                          /* proper-sum */
        }
    }
    size = (size_t)n + 1;   /* update stored size */

    return cache[(size_t)n];
}
