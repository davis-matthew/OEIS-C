/*
 * A183437  Number of n X 4 binary arrays with every 1 having exactly one
 *          king-move neighbor equal to 1.
 * https://oeis.org/A183437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 26*a(n-3) + 15*a(n-4) - 12*a(n-5) -
 *            52*a(n-6) - 28*a(n-7) - 18*a(n-8) + 7*a(n-9) (for n ≥ 10)
 *            Initial values (n = 1 … 9): 4, 28, 89, 361, 1620, 6392, 26243,
 *            109483, 447624
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache        = NULL;   /* cache[0] unused */
    static size_t cache_len = 0;          /* number of valid entries in cache */

    /* Sequence is undefined for n < 1 */
    if (n < 1)
        return (seq_t)0;

    /* Expand cache if needed */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;   /* need indices 0 … n */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)                 /* allocation failure */
            return (seq_t)0;
        cache = new_cache;

        /* Initial known terms (a(1) … a(9)) */
        static const seq_t init[10] = {
            (seq_t)0,                    /* placeholder for 0‑th index */
            (seq_t)4ULL,
            (seq_t)28ULL,
            (seq_t)89ULL,
            (seq_t)361ULL,
            (seq_t)1620ULL,
            (seq_t)6392ULL,
            (seq_t)26243ULL,
            (seq_t)109483ULL,
            (seq_t)447624ULL
        };

        /* Compute all entries up to new_len-1 */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            if (i <= 9)                      /* use the base case */
            {
                cache[i] = init[i];
            }
            else
            {
                /* Recurrence. All intermediate results are computed in
                   seq_t, so overflow is handled by the type itself. */
                seq_t sum = cache[i-1];
                sum += (seq_t)6 *  cache[i-2];
                sum += (seq_t)26 * cache[i-3];
                sum += (seq_t)15 * cache[i-4];
                sum -= (seq_t)12 * cache[i-5];
                sum -= (seq_t)52 * cache[i-6];
                sum -= (seq_t)28 * cache[i-7];
                sum -= (seq_t)18 * cache[i-8];
                sum += (seq_t)7  * cache[i-9];
                cache[i] = sum;
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
