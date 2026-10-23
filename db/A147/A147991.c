/*
 * A147991  Sequence S such that 1 is in S and if x is in S, then 3x-1 and
 *          3x+1 are in S.
 * https://oeis.org/A147991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A147991
   Formula: a(0) = 0, a(n) = 3*a(n/2)-1 if n is even,
              a(n) = 3*a((n-1)/2)+1 if n is odd.
   The offset for exported values is 1 (a(1) is the first term). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, indexed directly by n */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* length of allocated storage */
    static idx_t last_computed = 1;    /* highest index already computed */

    /* initialise base values a(0)=0, a(1)=1 */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)
            return 0;           /* allocation failure: return 0 as sentinel */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* return a(0) if that index is requested (used for recursion or checks) */
    if (n == 0)
        return cache[0];

    /* extend cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;                    /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp)
            return 0;           /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if ((i & 1) == 0)            /* i even */
            cache[i] = (seq_t)3 * cache[i / 2] - (seq_t)1;
        else                          /* i odd */
            cache[i] = (seq_t)3 * cache[(i - 1) / 2] + (seq_t)1;
    }
    last_computed = n;

    return cache[n];
}
