/*
 * A177785  a(n) = a(n-1)^2 - a(n-2) for n > 2; a(1)=3, a(2)=0.
 * https://oeis.org/A177785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=0, a(n)=a(n-1)^2 - a(n-2)  for n>2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of elements actually stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    if (n == 0)   /* undefined for n=0 – return 0 safely */
        return 0;

    /* Grow capacity if necessary – use doubling to keep O(n) overall */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < n) new_cap = n;   /* final size must be at least n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                     /* allocation failed – give up */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base terms if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;          /* a(1) */
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 2) {
        cache[1] = (seq_t)0;          /* a(2) */
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev     = cache[i - 2];    /* a(i-1) */
        seq_t prevprev = cache[i - 3];    /* a(i-2) */
        cache[i - 1] = prev * prev - prevprev;
    }
    cache_len = n;

    return cache[n - 1];
}
