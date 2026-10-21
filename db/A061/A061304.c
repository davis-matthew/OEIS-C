/*
 * A061304  Squarefree triangular numbers.
 * https://oeis.org/A061304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the n‑th square‑free triangular number
 * (the n‑th value of  t_k = k*(k+1)/2 such that t_k is square‑free)
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Cached terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* allocated space (elements) */
    static idx_t  cache_sz  = 0; /* number of stored terms   */
    static idx_t  last_k    = 0; /* largest k processed so far */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
    }

    /* Extend cache until it has at least n terms */
    while (cache_sz < n) {
        idx_t k = last_k + 1;
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;

        if (is_squarefree(t)) {
            /* Grow storage if necessary */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = t;
        }
        last_k = k;
    }

    return cache[n - 1]; /* 1‑based indexing */
}
