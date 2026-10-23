/*
 * A192253  1-sequence of reduction of (n!) by x^2 -> x+1.
 * https://oeis.org/A192253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*(n-3)*a(n-2) - (n-1)*(n-2)*a(n-3) with
 *            a(1)=0, a(2)=1, a(3)=3 (Recurrence from the OEIS entry A192253)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based indexing) */
    static seq_t *cache = NULL;   /* array of a(i) stored as cache[i-1] */
    static idx_t   cache_size = 0;/* number of terms already computed */
    static size_t  cache_cap   = 0;/* capacity of the cache array */

    /* The sequence is undefined for n < 1.  Return 0 for that case. */
    if (n < 1) return (seq_t)0;

    /* Ensure that the cache can hold the requested term. */
    if (cache_size < n) {
        /* Grow the cache if necessary. */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp && new_cap != 0) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute previously missing terms using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)0;
            } else if (i == 2) {
                val = (seq_t)1;
            } else if (i == 3) {
                val = (seq_t)3;
            } else {
                seq_t t1 = (seq_t)i * cache[i-2];                 /* n*a(n-1)    */
                seq_t t2 = (seq_t)(i-1) * (i-3) * cache[i-3];     /* (n-1)(n-3)a(n-2) */
                seq_t t3 = (seq_t)(i-1) * (i-2) * cache[i-4];     /* (n-1)(n-2)a(n-3) */
                val = t1 + t2 - t3;
            }
            cache[i-1] = val;
        }
        cache_size = n;
    }

    return cache[n-1];
}
