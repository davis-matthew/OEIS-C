/*
 * A028070  Expansion of 1/((1-3x)(1-5x)(1-9x)(1-12x)).
 * https://oeis.org/A028070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=29, a(2)=550, a(3)=8690;
   for n>=4: a(n)=29*a(n-1)-291*a(n-2)+1179*a(n-3)-1620*a(n-4) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)550;
    if (n == 3) return (seq_t)8690;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)550;
        cache[3] = (seq_t)8690;
        cache_size = 4;
    }

    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failure */
        cache = tmp;
        /* compute missing entries */
        for (size_t i = cache_size; i < needed; ++i) {
            cache[i] =
                (seq_t)29   * cache[i-1]          /* +29*a(n-1) */
              - (seq_t)291  * cache[i-2]         /* -291*a(n-2) */
              + (seq_t)1179 * cache[i-3]         /* +1179*a(n-3) */
              - (seq_t)1620 * cache[i-4];        /* -1620*a(n-4) */
        }
        cache_size = needed;
    }
    return cache[(size_t)n];
}
