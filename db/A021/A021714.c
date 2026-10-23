/*
 * A021714  Expansion of 1/((1-x)(1-3x)(1-10x)(1-11x)).
 * https://oeis.org/A021714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1/180 + 3^(n+3)/112 - 10^(n+3)/63 + 11^(n+3)/80
 *            (equivalently: a(n) = 25*a(n‑1) - 197*a(n‑2) + 503*a(n‑3) -
 *            330*a(n‑4) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* Allocate cache if needed and initialise first four terms */
    if (!cache) {
        idx_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* out of memory – should not happen */
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)428;
        cache[3] = (seq_t)6278;
        cache_size = 4;
    }

    /* Grow cache to accommodate requested index */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* out of memory – should not happen */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Compute required terms using the linear recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)25 * cache[i-1];
        val -= (seq_t)197 * cache[i-2];
        val += (seq_t)503 * cache[i-3];
        val -= (seq_t)330 * cache[i-4];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
