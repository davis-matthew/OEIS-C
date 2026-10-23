/*
 * A163472  a(n) = 12*a(n-1) - 33*a(n-2) for n > 1; a(0) = 3, a(1) = 21.
 * https://oeis.org/A163472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 33*a(n-2) for n > 1; a(0) = 3; a(1) = 21;
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t  len   = 0; /* number of cached terms */
    static idx_t  cap   = 0; /* allocated capacity */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cap = 2;
        len = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)21;
    }

    /* extend cache up to index n if necessary */
    if ((idx_t)n >= len) {
        idx_t needed = (idx_t)n + 1;
        if (needed > cap) {
            /* grow capacity to at least needed (use doubling strategy) */
            idx_t newcap = cap;
            while (newcap < needed) newcap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return 0;          /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }
        /* compute subsequent terms */
        for (size_t i = len; i < needed; ++i) {
            cache[i] = cache[i-1] * (seq_t)12 - cache[i-2] * (seq_t)33;
        }
        len = needed;
    }

    return cache[(size_t)n];
}
