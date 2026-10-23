/*
 * A138232  First differences of A138231.
 * https://oeis.org/A138232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 59, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138232:
   a(0)=1, a(1)=0, a(2)=0, a(3)=1;
   for n≥4:  a(n)=2*a(n-2)-2*a(n-4). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest index computed */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failed */
        cache[0] = 1; cache[1] = 0;
        cache[2] = 0; cache[3] = 1;
        cache_max = 3;
    }

    /* return if already computed */
    if (n <= cache_max) return cache[n];

    /* extend storage to accommodate index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failed */
    cache = tmp;

    /* compute terms from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 2] - (seq_t)2 * cache[i - 4];
    }
    cache_max = n;

    return cache[n];
}
