/*
 * A180086  Sum of n and floor of each previous term divided by its distance
 *          from n.
 * https://oeis.org/A180086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(a(n-1)/1) + floor(a(n-2)/2) + ... +
 *            floor(a(1)/(n-1))
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;          /* last index that has been computed (0 = none) */

    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    /* grow the cache to accommodate up to index n */
    if (n > cap)
    {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)                       /* allocation failed – return 0 */
            return 0;
        cache = tmp;
        cap   = n;
    }

    /* compute any missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k)
    {
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i)
        {
            /* cache index for a(k-i):  a(k-i) stored at cache[k-i-1] */
            sum += cache[(k - i) - 1] / (seq_t)i;
        }
        cache[k - 1] = (seq_t)k + sum;
    }
    last = n;
    return cache[n - 1];
}
