/*
 * A224500  Number of ordered full binary trees with labels from a set of at
 *          most n labels.
 * https://oeis.org/A224500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4,
   a(n) = (4*n-5)*a(n-1) - (4*n-4)*a(n-2) + 1   for n > 2 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* offset 1: a(0) is undefined, just return 0 for safety */
    if (n == 0) return 0;

    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* size of allocated array (indexable up to cache_cap-1) */
    static idx_t cached   = 0;    /* maximum index already computed in cache */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 3;                /* indices 0,1,2 will be used immediately */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 0;                 /* unused, but keeps indices aligned */
        cache[1] = 1;                 /* a(1) */
        cache[2] = 4;                 /* a(2) */
        cached   = 2;                 /* highest computed index */
    }

    /* ensure cache has space for up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        memcpy(new_arr, cache, (size_t)(cached + 1) * sizeof(seq_t));
        free(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)(4 * i - 5)) * cache[i - 1];
        seq_t term2 = ((seq_t)(4 * i - 4)) * cache[i - 2];
        cache[i] = term1 - term2 + 1;
    }

    cached = n;
    return cache[n];
}
