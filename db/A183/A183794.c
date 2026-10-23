/*
 * A183794  1/4 the number of (n+1) X 2 0..3 arrays with no 2 X 2 subblock
 *          having sum 6.
 * https://oeis.org/A183794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+81*a(n-2)+206*a(n-3)+144*a(n-4), a(1)=53,
 *            a(2)=706, a(3)=9395, a(4)=125050
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* offset is 1 – behaviour below is irrelevant */

    static seq_t *cache = NULL;         /* array index 0..cap-1 */
    static idx_t  cap   = 0;            /* allocated size */
    static idx_t  last  = 0;            /* highest index already computed */

    /* Allocate or grow cache to hold index n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* memory allocation failed – not expected */
        cache = tmp;
        cap   = new_cap;

        /* Initialise base cases (only once for the new or greatest cache size) */
        if (cap > 0)  cache[0] = (seq_t)0;           /* unused but convenient */
        if (cap > 1)  cache[1] = (seq_t)53;
        if (cap > 2)  cache[2] = (seq_t)706;
        if (cap > 3)  cache[3] = (seq_t)9395;
        if (cap > 4)  cache[4] = (seq_t)125050;

        if (last < 4) last = 4;
    }

    /* Compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1]
                     + (seq_t)81 * cache[i - 2]
                     + (seq_t)206 * cache[i - 3]
                     + (seq_t)144 * cache[i - 4];
        }
        last = n;
    }

    return cache[n];
}
