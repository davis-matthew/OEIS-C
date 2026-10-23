/*
 * A118408  Row sums of triangle A118407.
 * https://oeis.org/A118408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 33, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=-1, a(3)=1, a(4)=1
   for n≥5:  a(n) = -a(n-1) - a(n-2) + a(n-3) + 2*a(n-5)
   (derived from G.f. = (1+x)^2/((1+x^2)(1+2x+2x^2)(1-x))  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;           /* number of computed terms */

    /* initialise cache with the 5 initial terms */
    if (cache_len == 0) {
        cache_len = 5;                     /* indices 0..4 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;              /* abort gracefully */

        cache[0] = 1;                      /* a(0) */
        cache[1] = 1;                      /* a(1) */
        cache[2] = -1;                     /* a(2) */
        cache[3] = 1;                      /* a(3) */
        cache[4] = 1;                      /* a(4) */
    }

    /* if n is already computed, return it */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* extend cache to at least (n+1) elements */
    size_t old_len = cache_len;
    size_t new_len = n + 1;
    if (new_len <= old_len) {
        new_len = old_len * 2;            /* double to keep amortised O(1) */
        if (new_len <= old_len) new_len = old_len + 1; /* safety guard */
    }

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                     /* abort gracefully */
    cache = tmp;
    cache_len = new_len;

    /* compute any missing terms, starting from the first unused index */
    for (size_t i = old_len; i < new_len; ++i) {
        if (i < 5) {                     /* initial terms already set */
            continue;
        }
        /* recurrence: a(i) = -a(i-1)-a(i-2)+a(i-3)+2*a(i-5) */
        cache[i] = -cache[i-1] - cache[i-2] + cache[i-3] + (cache[i-5] << 1);
    }

    return cache[(size_t)n];
}
