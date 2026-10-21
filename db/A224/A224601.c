/*
 * A224601  Number of (n+2)X5 0..2 matrices with each 3X3 subblock idempotent.
 * https://oeis.org/A224601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a( n-5) -
 *            a(n-6), with a(1)=297, a(2)=371, a(3)=482, a(4)=571, a(5)=782,
 *            a(6)=1086, a(7)=1455, a(8)=2010, for n ≥ 1
 * Range:     u8 none, u16 n <= 17, u32 n <= 46, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1..8 */
    static const seq_t init[9] = {
        0,      /* unused index 0 */
        297,    /* a(1) */
        371,    /* a(2) */
        482,    /* a(3) */
        571,    /* a(4) */
        782,    /* a(5) */
        1086,   /* a(6) */
        1455,   /* a(7) */
        2010    /* a(8) */
    };

    /* Dynamic cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* allocated size in elements */
    static idx_t  maxn  = 0;        /* highest n that has been computed */

    /* Ensure cache is large enough for the requested n */
    if (!cache || (size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        if (newcap < 9) newcap = 9;           /* needs space for a[1..8] */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure; sequence undefined */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first eight entries once */
    if (maxn < 8) {
        for (size_t i = 1; i <= 8; ++i)
            cache[i] = init[i];
        maxn = 8;
    }

    /* Compute missing terms up to n if needed */
    if (n > maxn) {
        for (idx_t i = maxn + 1; i <= n; ++i) {
            seq_t v =  4 * cache[i - 1]
                     - 6 * cache[i - 2]
                     + 5 * cache[i - 3]
                     - 4 * cache[i - 4]
                     + 3 * cache[i - 5]
                     - cache[i - 6];
            cache[i] = v;
        }
        maxn = n;
    }

    return cache[n];
}
