/*
 * A016226  Expansion of 1/((1-x)(1-4x)(1-11x)).
 * https://oeis.org/A016226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016226: Expansion of 1/((1-x)(1-4x)(1-11x))
   Recurrence: a(0)=1, a(1)=16
               a(n)=15*a(n-1)-44*a(n-2)+1   (n>=2) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t old_size = cache_size;

    /* Grow cache to hold index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failed */

        /* Initialise new entries to zero (calloc‑like) */
        for (idx_t i = 0; i < new_size; ++i) new_cache[i] = (seq_t)0;

        /* Copy existing values */
        for (idx_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];

        /* Replace old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Set initial values if they are now available */
    if (cache_size >= 1) cache[0] = (seq_t)1;
    if (cache_size >= 2) cache[1] = (seq_t)16;

    /* Compute missing terms using the recurrence */
    idx_t start = old_size;
    if (start < 2) start = 2;           /* first term that needs the recurrence */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)15 * cache[i - 1]
                 - (seq_t)44 * cache[i - 2]
                 + (seq_t)1;
    }

    return cache[n];
}
