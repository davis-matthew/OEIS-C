/*
 * A231183  Coefficients of the nonnegative powers of rho(11) = 2*cos(Pi/11)
 *          when written in the power basis of the degree 5 number field
 *          Q(rho(11)). Negative of the coefficients of the first power.
 * https://oeis.org/A231183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5) with
 *            a(0)=0, a(1)=-1, a(2)=0, a(3)=0, a(4)=0
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;     /* array holding computed values */
static idx_t  cache_len = 0;    /* number of valid entries in cache (length) */

/* returns a(n) for any n >= 0, lazily computing and caching previous terms */
seq_t a(idx_t n)
{
    /* initialize cache for the first five terms if not yet done */
    if (cache_len == 0) {
        /* allocate space for at least 5 elements (indices 0..4) */
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();  /* out of memory: terminate */
        cache[0] = 0;
        cache[1] = (seq_t)(-1);
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
    }

    /* grow cache if n is beyond current capacity */
    if (n >= cache_len) {
        /* start with some capacity and grow as needed */
        idx_t new_cap = cache_len;
        while (n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
        cache_len = new_cap;
    }

    /* recompute values up to n if necessary */
    if (n >= 5) {
        /* compute all missing terms up to n */
        idx_t i;
        /* the cache currently contains values up to cache_len-1
           but may already have computed some of the needed indices
           (i.e., up to some last_computed < 5 or < n).  We'll
           compute from the currently stored highest index + 1. */
        idx_t start = 5;
        /* Find the first index < 5 that might already be computed? 
           Since we initialized all up to 4, we start from 5. */
        for (i = 5; i <= n; ++i) {
            cache[i] = cache[i-1] + 4*cache[i-2] - 3*cache[i-3]
                       - 3*cache[i-4] + cache[i-5];
        }
    }

    /* return the requested term */
    return cache[n];
}
