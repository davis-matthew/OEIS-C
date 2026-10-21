/*
 * A086928  a(n) = 12*a(n-1) + a(n-2), with a(0) = 2 and a(1) = 12.
 * https://oeis.org/A086928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1) + a(n-2), a(0)=2, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized table */
    static idx_t cap   = 0;       /* current capacity */
    static idx_t len   = 0;       /* number of filled entries */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2;                                 /* store a(0) and a(1) */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)12;
        len = 2;
    }

    /* early return if already computed */
    if (n < len)
        return cache[n];

    /* grow the cache if needed */
    if (n >= cap) {
        idx_t newcap = cap;
        while (n >= newcap)
            newcap *= 2;                      /* simple exponential growth */
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = (seq_t)(cache[i-1] * (seq_t)12 + cache[i-2]);
    }
    len = n + 1;

    return cache[n];
}
