/*
 * A026179  Numbers k such that A026177(j) < A026177(k) for all j < k.
 * https://oeis.org/A026179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest integer of the form (3*i+2)*3^j * for i,j ≥
 *            0, with a(1) = 1. * This implements the definition from OEIS
 *            A026179
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    /* The next integer to test for membership in the set */
    static seq_t next_test      = (seq_t)2;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;    /* a(1) */
        cache_len = 1;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = next_test;
        /* Find the next integer belonging to the set starting at cand */
        for (;;) {
            seq_t r = cand;
            /* remove all factors of 3 */
            while (r % (seq_t)3 == (seq_t)0)
                r /= (seq_t)3;
            if (r % (seq_t)3 == (seq_t)2)
                break;                /* cand is of the required form */
            ++cand;
        }

        /* Store the found term */
        if (cache_len == cache_cap) {             /* grow cache if needed */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cand;

        next_test = cand + (seq_t)1;              /* next candidate to test */
    }

    return cache[n - 1];   /* 1‑based indexing */
}
