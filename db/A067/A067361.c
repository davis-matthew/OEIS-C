/*
 * A067361  a(n) = 17^n*cos(2*n*arctan(1/4)) or denominator of
 *          tan(2*n*arctan(1/4)).
 * https://oeis.org/A067361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(1)=15, a(2)=161, a(n)=30*a(n-1)-289*a(n-2) for n>2
 *            (Equivalently a(n)=17^n*cos(2*n*arctan(1/4)) )
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 7, i64 n <= 16, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized recurrence */
    static seq_t *cache = NULL;        /* array of a(1)…a(last_computed) */
    static idx_t cache_size = 0;       /* allocated size of cache */
    static idx_t last_computed = 0;    /* number of terms already stored */

    if (n == 0)
        return 0;                      /* undefined for n==0 – return 0 */

    /* Ensure cache can hold n terms (n is 1‑based) */
    if ((idx_t)n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_size = n;
    }

    /* Compute any missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = 15;
        else if (i == 2)
            val = 161;
        else
            val = (seq_t)30 * cache[i - 2] - (seq_t)289 * cache[i - 3];
        cache[i - 1] = val;           /* cache index 0 corresponds to a(1) */
    }
    last_computed = n;
    return cache[n - 1];
}
