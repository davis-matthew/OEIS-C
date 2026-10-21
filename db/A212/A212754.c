/*
 * A212754  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w<R, x>R, y>R, z>R, where R =
 *          max{w,x,y,z} - min{w,x,y,z}.
 * https://oeis.org/A212754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A212754: Number of (w,x,y,z) with all terms in {0,…,n}
   and at least one of the conditions w<R, x>R, y>R, z>R holds,
   where R = max{w,x,y,z} - min{w,x,y,z}.
   Recurrence: a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
   with initial values:
        a(0)=0, a(1)=8, a(2)=53, a(3)=184, a(4)=472, a(5)=1008
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for fast random access after the first pass */
    static seq_t *cache   = NULL;      /* array storing computed terms      */
    static idx_t  size    = 0;          /* current allocated array size      */
    static idx_t  max     = 0;          /* largest index cached so far       */
    static const seq_t init[6] = {0, 8, 53, 184, 472, 1008};

    /* trivial case: the first six terms are known */
    if (n < 6)
        return init[(size_t)n];

    /* allocate or enlarge the cache if needed */
    if (cache == NULL) {
        size = n + 1;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return 0;           /* out-of-memory – return 0 silently */
        for (idx_t i = 0; i < 6; ++i)   /* initialise known terms */
            cache[i] = init[i];
        max = 5;
    } else if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;             /* out-of-memory */
        cache = tmp;
        size  = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * 4;   /* 4*a(n-1) */
        val -= cache[i - 2] * 5;        /* -5*a(n-2) */
        val += cache[i - 4] * 5;        /* +5*a(n-4) */
        val -= cache[i - 5] * 4;        /* -4*a(n-5) */
        val += cache[i - 6];            /* + a(n-6) */
        cache[i] = val;
    }
    max = n;
    return cache[n];
}
