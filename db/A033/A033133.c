/*
 * A033133  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-3) - 6*a(n-4); a(1)=1, a(2)=7, a(3)=42,
 *            a(4)=253
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* out of bounds – never requested */

    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of values stored (a(1) at index 0) */

    /* Return a cached value if already computed */
    if (n <= cached_size)
        return cache[n - 1];

    /* Grow the cache to accommodate up to a(n) */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache)
        abort();    /* allocation failed */
    cache = new_cache;

    /* Compute any missing terms */
    for (idx_t i = cached_size + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
            val = 1;
        else if (i == 2)
            val = 7;
        else if (i == 3)
            val = 42;
        else if (i == 4)
            val = 253;
        else
            val = 6 * cache[i - 2] + cache[i - 4] - 6 * cache[i - 5];

        cache[i - 1] = val;
    }

    cached_size = n;
    return cache[n - 1];
}
