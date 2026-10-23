/*
 * A184369  1/3 the number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          having at least two equal elements connected horizontally or
 *          vertically.
 * https://oeis.org/A184369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184369
   Empirical recurrence:
   a(1)=153, a(2)=2755, a(3)=49235, a(4)=880781,
   a(n)=17*a(n-1)+19*a(n-2)-57*a(n-3)+18*a(n-4)   (n≥5)
   Offset 1 (a(1) is first term).  */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* a(0) is undefined for the offset-1 sequence; return 0 if called */
    if (n == 0) return 0;

    /* static cache of already computed terms, 0‑based indices */
    static seq_t *cache = NULL;    /* pointer to allocated array */
    static idx_t cache_len = 0;    /* number of values stored */

    /* expand cache if necessary */
    if (cache_len < n) {
        /* reallocate to hold n terms (size_t is safe for n ≤ 10000) */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – clear cache */
            free(cache);
            cache   = NULL;
            cache_len=0;
            return 0;
        }
        cache = tmp;

        /* initialise first four terms if they are not yet present */
        if (cache_len == 0) {
            cache[0] = (seq_t)153;
            if (n > 1) cache[1] = (seq_t)2755;
            if (n > 2) cache[2] = (seq_t)49235;
            if (n > 3) cache[3] = (seq_t)880781;
        } else {
            if (cache_len <= 0 && n > 0) cache[0] = (seq_t)153;
            if (cache_len <= 1 && n > 1) cache[1] = (seq_t)2755;
            if (cache_len <= 2 && n > 2) cache[2] = (seq_t)49235;
            if (cache_len <= 3 && n > 3) cache[3] = (seq_t)880781;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 4) continue;   /* already initialised */
            seq_t t0 = cache[i-4];
            seq_t t1 = cache[i-3];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-1];
            seq_t val = (seq_t)17 * t3
                      + (seq_t)19 * t2
                      - (seq_t)57 * t1
                      + (seq_t)18 * t0;
            cache[i] = val;
        }
        cache_len = n;
    }

    /* convert 1‑based index to 0‑based array index */
    return cache[n - 1];
}
