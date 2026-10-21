/*
 * A089983  1, 1, 1, 1, ... a, b, c, d, ab-cd, ...
 * https://oeis.org/A089983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4)*a(n-3) - a(n-2)*a(n-1), with a(1)=a(2)=a(3)=a(4)=1
 *            (offset 1)
 * Range:     i8 n <= 14, i16 n <= 15, i32 n <= 17, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t last = 0;           /* largest index already computed */
    if (n <= last)
        return cache[n];            /* already known */

    /* grow the cache to hold indices up to n */
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache)                      /* allocation failure – abort */
        exit(1);

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 4)
            cache[i] = (seq_t)1;
        else
            cache[i] = cache[i-4] * cache[i-3] - cache[i-2] * cache[i-1];
    }
    last = n;
    return cache[n];
}
