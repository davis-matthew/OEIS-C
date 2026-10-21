/*
 * A233218  Number of 2 X n 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 3 and 4, and 2 appearing before 3
 *          in row major order.
 * https://oeis.org/A233218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=23, a(3)=376, a(4)=7222 For n>4 :
 *            a(n)=29*a(n-1)-175*a(n-2)+147*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1..] holds computed values */
    static idx_t sz = 0;          /* maximum index currently stored */

    if (n == 0) return 0;         /* undefined for n=0, but harmless */

    /* if already computed, return immediately */
    if (n <= sz)
        return cache[n];

    /* remember previous size before expansion */
    idx_t old_sz = sz;

    /* we always keep the cache at least up to 4 for initial values */
    sz = (n > 4) ? n : 4;

    /* grow the cache array */
    cache = (seq_t *)realloc(cache, (sz + 1) * sizeof(seq_t));
    if (!cache) return 0;         /* allocation failure, should not happen */

    /* initialize the first four values on first use */
    if (old_sz == 0) {
        cache[0] = 0;            /* unused */
        cache[1] = 3;
        cache[2] = 23;
        cache[3] = 376;
        cache[4] = 7222;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = old_sz + 1; i <= n; ++i) {
        if (i <= 4) continue;    /* initial values already set */
        cache[i] = 29 * cache[i-1]
                 - 175 * cache[i-2]
                 + 147 * cache[i-3];
    }

    return cache[n];
}
