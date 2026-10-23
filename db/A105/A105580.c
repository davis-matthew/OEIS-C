/*
 * A105580  a(n+3) = a(n) - a(n+1) - a(n+2); a(0) = -5, a(1) = 6, a(2) = 0.
 * https://oeis.org/A105580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+3) = a(n) - a(n+1) - a(n+2); a(0) = -5, a(1) = 6, a(2) = 0
 * Range:     i8 n <= 12, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  size  = 0;   /* number of values actually stored */
    static idx_t  capa  = 0;   /* allocated capacity */

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        capa = 4;                     /* small starting capacity */
        cache = (seq_t *)malloc(capa * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed, return 0 */

        cache[0] = (seq_t)-5;
        cache[1] = (seq_t) 6;
        cache[2] = (seq_t) 0;
        size    = 3;
    }

    /* extend the cache until it contains a(n) */
    while (size <= n) {
        seq_t val = cache[size - 3] - cache[size - 1] - cache[size - 2];

        /* grow the buffer if necessary */
        if (size >= capa) {
            capa *= 2;                  /* exponential growth */
            cache = (seq_t *)realloc(cache, capa * sizeof(seq_t));
            if (!cache) {
                /* allocation failed; keep the old buffer to avoid crash */
                return (seq_t)0;
            }
        }

        cache[size] = val;
        size++;
    }

    return cache[n];
}
