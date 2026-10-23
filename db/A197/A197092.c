/*
 * A197092  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,4,0,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-4) with a(1)=1, a(2)=7,
 *            a(3)=18, a(4)=55
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n<1 (not part of the defined sequence) */
    if (n == 0) return (seq_t)0;

    /* For the first four indices we return the known constants directly. */
    if (n <= 4) {
        static const seq_t base[5] = { (seq_t)0, (seq_t)1, (seq_t)7,
                                       (seq_t)18, (seq_t)55 };
        return base[n];
    }

    /* Static cache for memoization.  The cache is 1‑based; index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated size (number of elements)   */
    static idx_t  last = 0;   /* highest index that has already been computed */

    /* Ensure the cache can hold at least indices up to n. */
    if (n + 1 > cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (this situation should not occur in
               normal use). */
            return (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* Initialise the first four values once.  They are needed for the
       recurrence. */
    if (last == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)55;
        last = 4;
    }

    /* If the requested value has already been computed, just return it. */
    if (n <= last)
        return cache[n];

    /* Compute new values using the recurrence. */
    idx_t i;
    for (i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1]
                  + (seq_t)3 * cache[i - 2]
                  -          cache[i - 4];
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
