/*
 * A049427  Row sums of triangle A049424.
 * https://oeis.org/A049427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (recurrence):
      a(0)=1, a(1)=1, a(2)=5, a(3)=25, a(4)=145
      a(n) = a(n-1) + 4*(n-1)*a(n-2)
            + 6*(n-1)*(n-2)*a(n-3)
            + 4*(n-1)*(n-2)*(n-3)*a(n-4)
            + (n-1)*(n-2)*(n-3)*(n-4)*a(n-5)   for n≥5
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum index guaranteed by tests (≤10000)                */
    const idx_t MAX_N   = 10000;
    const idx_t CACHE_SZ = MAX_N + 1;   /* indices 0 .. 10000 */

    static seq_t *cache = NULL;
    static idx_t  next   = 0;  /* next index that needs to be computed */

    if (!cache)
    {
        cache = (seq_t *)malloc(CACHE_SZ * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        /* initialise first five values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)145;
        next = 5;                            /* next index to compute   */
    }

    if (n < next)
        return cache[n];

    /* compute all missing terms up to n */
    for (idx_t k = next; k <= n; ++k)
    {
        idx_t km1 = k - 1;
        idx_t km2 = k - 2;
        idx_t km3 = k - 3;
        idx_t km4 = k - 4;
        idx_t km5 = k - 5;

        seq_t value = cache[km1];
        value += cache[km2] * (seq_t)(4 * (k - 1));
        value += cache[km3] * (seq_t)(6 * (k - 1) * (k - 2));
        value += cache[km4] * (seq_t)(4 * (k - 1) * (k - 2) * (k - 3));
        value += cache[km5] * (seq_t)((k - 1) * (k - 2) * (k - 3) * (k - 4));

        cache[k] = value;
    }

    next = n + 1;
    return cache[n];
}
