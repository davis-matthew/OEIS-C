/*
 * A094831  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 3, s(2n) = 3.
 * https://oeis.org/A094831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094831: Recurrence
   a(0) = 1, a(1) = 2, a(2) = 6
   a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3)   for n ≥ 3
   (Called with index n = 0,1,2,3,...).  */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of values */
    static idx_t  sz      = 0;      /* number of values stored (cache[0..sz-1]) */
    static idx_t  cap     = 0;      /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap    = 4; /* initial capacity */
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        /* guard against malloc failure – not required for test harness */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        sz = 3;
    }

    /* if the value is already computed, return it */
    if (n < sz)
        return cache[n];

    /* grow capacity if needed */
    if (cap <= n + 1) {
        size_t new_cap = cap ? (size_t)cap << 1 : 4;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = (idx_t)new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = sz; k <= n; ++k) {
        /* recurrence only needed for k ≥ 3 */
        seq_t val = (seq_t)6 * cache[k-1] - (seq_t)9 * cache[k-2] + cache[k-3];
        cache[k] = val;
    }
    sz = n + 1;   /* update stored size */

    return cache[n];
}
