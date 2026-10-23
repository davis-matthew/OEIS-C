/*
 * A197469  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,1,0,0,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) for n > 6; * Initial values: a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=12, a(5)=24, a(6)=50
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;
    static idx_t cached = 0;  /* number of terms stored in cache */

    /* Return cached result if available */
    if (n <= cached)
        return cache[n - 1];

    idx_t newsize = n;
    seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newcache)
        return 0;  /* allocation failure */

    /* Base cases */
    if (newsize >= 1) newcache[0] = (seq_t)1;   /* a(1) */
    if (newsize >= 2) newcache[1] = (seq_t)2;   /* a(2) */
    if (newsize >= 3) newcache[2] = (seq_t)6;   /* a(3) */
    if (newsize >= 4) newcache[3] = (seq_t)12;  /* a(4) */
    if (newsize >= 5) newcache[4] = (seq_t)24;  /* a(5) */
    if (newsize >= 6) newcache[5] = (seq_t)50;  /* a(6) */

    /* Recurrence for n > 6 */
    for (idx_t i = 7; i <= n; ++i) {
        newcache[i - 1] = newcache[i - 2] + ((seq_t)4 * newcache[i - 4]);
        /* a(i) stored at index i-1 */
    }

    cache = newcache;
    cached = newsize;

    return cache[n - 1];
}
