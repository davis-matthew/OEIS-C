/*
 * A144739  7-factorial numbers A114799(7*n+3): Partial products of A017017(k)
 *          = 7*k+3, a(0) = 1.
 * https://oeis.org/A144739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = (7*n-4)*a(n-1) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* previously computed terms   */
    static idx_t  maxn   = 0;     /* highest index present       */
    static size_t cap    = 0;     /* allocated capacity          */

    /* Initialise on first call.  Allocate one element for a(0). */
    if (!cache) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* memory error → sentinel */
        cache[0] = 1;                   /* a(0) = 1                 */
        maxn = 0;
    }

    /* Extend cache and compute missing terms if n is larger. */
    if (n > maxn) {
        if ((idx_t)cap <= n) {
            size_t newcap = cap;
            while ((idx_t)newcap <= n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;        /* memory error → sentinel */
            cache = tmp;
            cap = newcap;
        }

        for (idx_t i = maxn + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)(7 * i - 4);
        }
        maxn = n;
    }

    return cache[n];
}
