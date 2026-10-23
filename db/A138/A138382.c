/*
 * A138382  First differences of A138380. Second differences of A138377.
 * https://oeis.org/A138382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4 * a(n-4) for n >= 4 Initial values: a(0)=1, a(1)=-3,
 *            a(2)=-1, a(3)=-2
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cached sequence */
    static seq_t *cache   = NULL;   /* array holding computed terms */
    static size_t cache_size = 0;   /* allocated capacity */
    static idx_t  cached_upto = (idx_t)-1; /* highest index stored (-1 means none) */

    /* allocate or grow array to hold indices 0..n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (cached_upto == (idx_t)-1 || n > cached_upto) {
        idx_t start = (cached_upto == (idx_t)-1) ? 0 : cached_upto + 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)-3;
            else if (i == 2)     cache[i] = (seq_t)-1;
            else if (i == 3)     cache[i] = (seq_t)-2;
            else                 cache[i] = (seq_t)-4 * cache[i - 4];
        }
        cached_upto = n;
    }

    return cache[n];
}
