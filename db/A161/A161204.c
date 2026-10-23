/*
 * A161204  a(0)=2. a(n+1) = 2*a(n) + period 4: repeat -5,1,3,1.
 * https://oeis.org/A161204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(0) = 2,  a(n+1) = 2*a(n) + p[(n) mod 4] 
 * where p = {-5, 1, 3, 1}.  This generates A161204.
 */
#include <stdlib.h>

static const int p[4] = {-5, 1, 3, 1};

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* allocated array of seq_t */
    static idx_t  cap     = 0;      /* current capacity (number of slots) */
    static idx_t  last    = 0;      /* largest index already computed (>=0) */

    /* initialise cache on first call */
    if (!cache) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed, return 0 */
        cache[0] = (seq_t)2;          /* a(0) */
        last = 0;
    }

    /* if requested index is greater than what we have, extend and compute */
    if (n > last) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[last];   /* realloc failed – return last value */
        cache = tmp;
        cap   = new_cap;

        /* compute missing terms from last+1 to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            int pat = p[(i - 1) & 3];          /* pattern for term a(i) */
            cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)pat;
        }
        last = n;
    }

    return cache[n];
}
