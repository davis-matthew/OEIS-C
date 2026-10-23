/*
 * A049864  a(n) = Sum_{k=0,1,2,...,n-4,n-2,n-1} a(k); a(n-3) is not a
 *          summand, with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A049864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4) with initial values
 *            a(0)=a(1)=a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding already computed terms. */
    static seq_t *cache   = NULL;   /* pointer to array of values */
    static size_t cap    = 0;       /* current allocated size */
    static idx_t  used   = 0;       /* number of values computed (next free index) */

    /* Grow the cache if it cannot hold a(n). */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(*cache));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = used; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 1;
        } else if (i == 3) {
            cache[i] = 2;
        } else {
            /* Recurrence relation from OEIS:
               a(i) = 2*a(i-1) - a(i-3) + a(i-4)   (valid for i ≥ 4) */
            cache[i] = 2 * cache[i-1] - cache[i-3] + cache[i-4];
        }
    }
    used = n + 1;

    return cache[n];
}
