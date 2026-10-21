/*
 * A093145  Third binomial transform of Fibonacci(3n)/Fibonacci(3).
 * https://oeis.org/A093145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 20*a(n-2)    (n > 1)
 *          a(0) = 0, a(1) = 1
 *
 * The sequence satisfies this second‑order linear recurrence with constant
 * coefficients, allowing fast computation via incremental evaluation.
 * A static cache is used so that calls in any order are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size)
        return cache[n];

    /* expand cache to hold values up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return 0; /* allocation failure – unlikely for n <= 10000 */

    cache = new_cache;
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 1;
        else
            cache[i] = 10 * cache[i - 1] - 20 * cache[i - 2];
    }

    cache_size = new_size;
    return cache[n];
}
