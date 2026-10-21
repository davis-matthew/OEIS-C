/*
 * A086026  a(n) = Sum_{i=1..n} C(i+4,5)^3.
 * https://oeis.org/A086026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+4,5)^3 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 44, u128 n <= 744,
 *            big any n
 * Helpers:   binomial_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial_small.h"

/* ------------------------------------------------------------------
 *  a:  compute sequence A086026.
 *  Caches values so that repeated calls with arbitrary indices run in
 *  O(k) where k is the distance from the largest previously computed
 *  index.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(0) is defined only for safety; the sequence is 1-indexed. */
    if (n < 1) return (seq_t)0;

    /* dynamic cache: cache[i] holds a(i).  Index 0 is unused but kept. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_max   = 0;

    /* Allocate or grow the cache if necessary. */
    if (!cache || n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                     /* allocation failed: return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed. */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms incrementally. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t c   = binomial_small(i + 4, 5);   /* C(i+4,5) */
        seq_t cube = c * c * c;                 /* its cube */
        cache[i] = cache[i - 1] + cube;         /* accumulate sum */
    }

    cache_max = n;
    return cache[n];
}
