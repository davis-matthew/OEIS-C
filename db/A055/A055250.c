/*
 * A055250  Seventh column of triangle A055249.
 * https://oeis.org/A055250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula for A055250 (7th column of triangle A055249):
   a(0) = 1, a(1) = 9, a(2) = 47, a(3) = 187, a(4) = 630,
   a(5) = 1898, a(6) = 5282.
   For n ≥ 7:
   a(n) = 9*a(n-1) – 34*a(n-2) + 70*a(n-3) – 85*a(n-4)
          + 61*a(n-5) – 24*a(n-6) + 4*a(n-7).          */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  size   = 0;    /* number of computed terms */
    static size_t  cap    = 0;    /* allocated capacity */

    /* initial seven terms */
    static const seq_t init[7] = {
        (seq_t)1,  (seq_t)9,  (seq_t)47,
        (seq_t)187, (seq_t)630, (seq_t)1898,
        (seq_t)5282
    };
    const size_t init_len = sizeof(init)/sizeof(init[0]);

    /* initialise cache on first call */
    if (!cache) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        size = init_len;
    }

    /* if requested index already computed, return it */
    if ((size_t)n < size)
        return cache[(size_t)n];

    /* extend cache up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        /* grow memory if necessary */
        if (i >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {                     /* allocation failed */
                free(cache);
                cache = NULL;
                size  = 0;
                cap   = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        if (i < init_len) {
            /* initial terms are already stored */
            continue;
        }

        /* recurrence computation */
        seq_t val = (seq_t)9  * cache[i-1]
                 - (seq_t)34 * cache[i-2]
                 + (seq_t)70 * cache[i-3]
                 - (seq_t)85 * cache[i-4]
                 + (seq_t)61 * cache[i-5]
                 - (seq_t)24 * cache[i-6]
                 + (seq_t)4  * cache[i-7];
        cache[i] = val;
    }

    size = (size_t)n + 1;
    return cache[(size_t)n];
}
