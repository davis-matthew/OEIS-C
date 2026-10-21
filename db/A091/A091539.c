/*
 * A091539  Second column (k=3) of array A091534 ((5,2)-Stirling2) divided by
 *          10.
 * https://oeis.org/A091539
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*(6*n-5)*a(n-1) -
 *            3*(3*n-5)*(n-1)*(3*n-1)*(3*n-4)*a(n-2) initial values: a(2)=1,
 *            a(3)=104
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined for the offset */

    static seq_t *cache = NULL;         /* memoisation table */
    static idx_t  cap   = 0;            /* allocated size of cache       */
    static idx_t  maxn  = 1;            /* highest n already computed     */

    /* grow cache if necessary */
    if (!cache || cap <= n) {
        idx_t newcap = cap ? cap : 4;
        while (newcap <= n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);                    /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise first two terms once */
    if (maxn < 2) {
        cache[2] = (seq_t)1;            /* a(2) */
        cache[3] = (seq_t)104;          /* a(3) */
        maxn    = 3;
    }

    /* compute missing terms up to n */
    if (n > maxn) {
        for (idx_t i = maxn + 1; i <= n; ++i) {
            /* a(i) = (3*i-1)*(6*i-5)*a(i-1)
                     - 3*(3*i-5)*(i-1)*(3*i-1)*(3*i-4)*a(i-2) */
            seq_t t1 = cache[i-1];
            t1 *= (seq_t)(3*i-1);
            t1 *= (seq_t)(6*i-5);

            seq_t t2 = cache[i-2];
            t2 *= (seq_t)(i-1);          /* (n-1) */
            t2 *= (seq_t)(3*i-5);
            t2 *= (seq_t)(3*i-1);
            t2 *= (seq_t)(3*i-4);
            t2 *= (seq_t)3;

            cache[i] = t1 - t2;
        }
        maxn = n;
    }

    return cache[n];
}
