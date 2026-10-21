/*
 * A236583  The number of tilings of an 8 X (3n) floor with 2 X 3 hexominoes.
 * https://oeis.org/A236583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 3*a(n-1) - a(n-3) +
 *            a(n-4) for n ≥ 4, with initial values a(0)=1, a(1)=1, a(2)=4,
 *            a(3)=11. The generating function is (-1+x)^2/(x^3 - x^4 + 1 -
 *            3x)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* cache size is sufficient for all n up to about 10,000 */
    static seq_t cache[10001];
    static size_t cache_len = 0;   /* number of computed values in cache */

    /* If n is already computed, return it immediately */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute values up to n, extending the cache if necessary */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1)       cache[i] = (seq_t)1;
        else if (i == 2)       cache[i] = (seq_t)4;
        else if (i == 3)       cache[i] = (seq_t)11;
        else                   /* recurrence for i ≥ 4 */
            cache[i] = (seq_t)3 * cache[i-1] - cache[i-3] + cache[i-4];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
