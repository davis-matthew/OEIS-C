/*
 * A211542  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w=4y-3x.
 * https://oeis.org/A211542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-5) - a(n-6) - a(n-7) +
 *            a(n-9) Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=3,
 *            a(5)=5, a(6)=8, a(7)=10, a(8)=14
 * Range:     u8 n <= 33, u16 n <= 535, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..8 */
    static const seq_t base[9] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)5, (seq_t)8, (seq_t)10, (seq_t)14 };

    /* Return a base value directly for n < 9 */
    if (n < 9)
        return base[n];

    /* Static cache for computed values */
    static seq_t *cache   = NULL;  /* cache[0..cap-1] holds a(0) .. a(cap-1) */
    static idx_t  cap     = 0;     /* current capacity of the cache */
    static idx_t  last    = 0;     /* largest index that has been computed */

    /* Ensure that the cache is large enough for indices 0..n */
    if (cap <= n) {
        idx_t newcap = cap ? cap : 9;
        while (newcap <= n)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Make sure the base entries are present in the cache */
    if (last < 8) {
        for (idx_t i = 0; i <= 8; ++i)
            cache[i] = base[i];
        last = 8;   /* we have computed up to index 8 */
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Recurrence uses only indices that are already computed
           because i >= 9 and last >= i-1 during the loop */
        seq_t val =  cache[i-2] + cache[i-3] + cache[i-4]
                    - cache[i-5] - cache[i-6] - cache[i-7]
                    + cache[i-9];
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
