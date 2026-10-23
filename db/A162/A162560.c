/*
 * A162560  a(n) = (4+sqrt(3))*(3+sqrt(3))^n + (4-sqrt(3))*(3-sqrt(3))^n.
 * https://oeis.org/A162560
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2), a(-1)=3, a(0)=8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A162560 */
/* Requires external types: idx_t (int64_t) and seq_t (unsigned integer or big integer) */
#include <stdlib.h>      /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache: indices from MIN_IDX to MAX_IDX are stored in cache[0..size-1] */
    static seq_t *cache = NULL;
    static size_t size = 0;
    static idx_t min_idx = -1;   /* always -1 for this sequence */
    static idx_t max_idx = -1;   /* highest index computed so far */

    /* Helper to convert sequence index to cache offset */
    #define OFF(i) ((size_t)((i) - min_idx))

    /* Initialisation on first call */
    if (cache == NULL) {
        size = 2;                      /* store terms for -1 and 0 */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* fallback on allocation failure */
        cache[0] = (seq_t)3;           /* a(-1) */
        cache[1] = (seq_t)8;           /* a(0)  */
        max_idx = 0;
    }

    /* If requested index is within cached range, return directly */
    if (n <= max_idx) {
        return cache[OFF(n)];
    }

    /* Grow cache to accommodate new indices up to n */
    idx_t new_max = n;
    size_t new_size = (size_t)(new_max - min_idx + 1);
    if (new_size > size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* fallback on allocation failure */
        cache = tmp;
        /* Zero new elements to support computation */
        for (size_t i = size; i < new_size; ++i) cache[i] = 0;
        size = new_size;
    }

    /* Compute missing terms iteratively */
    for (idx_t i = max_idx + 1; i <= new_max; ++i) {
        seq_t prev1 = cache[OFF(i - 1)];
        seq_t prev2 = cache[OFF(i - 2)];
        cache[OFF(i)] = 6 * prev1 - 6 * prev2;
    }

    max_idx = new_max;
    return cache[OFF(n)];

    #undef OFF
}
