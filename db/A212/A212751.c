/*
 * A212751  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w=R, x=R, y=R, z<R, where R =
 *          max{w,x,y,z} - min{w,x,y,z}.
 * https://oeis.org/A212751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212751: a(n) = number of (w,x,y,z) with all terms in {0,…,n}
   satisfying at least one of the conditions
        w = R,  x = R,  y = R,  z < R
   where R = max{w,x,y,z} – min{w,x,y,z}.

   The recurrence is
        a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n+8)
   with initial values
        a(0)=1, a(1)=14, a(2)=70, a(3)=198, a(4)=477,
        a(5)=924, a(6)=1696, a(7)=2768.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of allocated slots */
    static idx_t last_computed = (idx_t)-1;   /* greatest index already filled */

    /* Allocate or grow cache to at least 8 elements – required for the base case */
    if (cache_len < 8) {
        size_t newlen = 8;
        seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise first eight values */
        cache[0] = 1;    cache[1] = 14;
        cache[2] = 70;   cache[3] = 198;
        cache[4] = 477;  cache[5] = 924;
        cache[6] = 1696; cache[7] = 2768;
        cache_len = 8;
        last_computed = 7;
    }

    /* Grow cache if we need more space for n */
    if (n >= cache_len) {
        size_t newlen = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise any newly allocated entries to zero – they will be overwritten */
        for (idx_t i = cache_len; i < newlen; ++i) cache[i] = 0;
        cache_len = newlen;
    }

    /* Compute values up to n if necessary */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term = 0;
            term += (seq_t)2 * cache[i-1];
            term += (seq_t)2 * cache[i-2];
            term -= (seq_t)6 * cache[i-3];
            term += (seq_t)6 * cache[i-5];
            term -= (seq_t)2 * cache[i-6];
            term -= (seq_t)2 * cache[i-7];
            term += cache[i-8];
            cache[i] = term;
        }
        last_computed = n;
    }

    return cache[n];
}
