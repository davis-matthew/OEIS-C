/*
 * A140780  a(n) = 10*a(n-1) - a(n-2) for n >= 2 with a(0) = 1 and a(1) = 3.
 * https://oeis.org/A140780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots */

    /* Base cases: handled directly to avoid touching cache */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /*--- Ensure we have a cache large enough to hold up to a(n) ---*/
    if (cache_size <= n)
    {
        idx_t old_size = cache_size;
        /* Grow size to at least n+1 (doubling for amortised O(n)) */
        idx_t new_size = (cache_size == 0) ? (n + 1) : cache_size;
        while (new_size <= n) new_size <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* Allocation failed: return 0 */
        cache = tmp;
        cache_size = new_size;

        /* Initialise the first two terms if this is the first allocation */
        if (old_size == 0)
        {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)3;
        }
        /* If cache was previously smaller than 2, we must set term 1 */
        if (old_size < 2)
            cache[1] = (seq_t)3;
    }

    /*--- Compute any missing terms up to a(n) ---*/
    idx_t start = (cache_size > 2) ? 2 : cache_size;
    /* if start == 0 or 1, we skip to 2 because those are already set */
    for (idx_t i = start; i <= n; ++i)
    {
        /* a(i) = 10*a(i-1) - a(i-2) */
        seq_t val = cache[i - 1];
        val = val * (seq_t)10 - cache[i - 2];
        cache[i] = val;
    }

    /* Adjust logical size of cached data, not the allocated size */
    if (cache_size < n + 1)
        cache_size = n + 1;

    return cache[n];
}
