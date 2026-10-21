/*
 * A221365  The simple continued fraction expansion of F(x) := Product_{n >=
 *          0} (1 - x^(4*n+3))/(1 - x^(4*n+1)) when x = 1/2*(5 - sqrt(21)).
 * https://oeis.org/A221365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A221365  –  Simple continued fraction expansion terms.
    Recurrence (for n ≥ 6):
        a(n) = 6*a(n-2) - 6*a(n-4) + a(n-6)
    Initial values:
        a(0) = 1,  a(1) = 3,  a(2) = 1,  a(3) = 21,
        a(4) = 1,  a(5) = 108
    This implementation memoises results in a static array so that
    repeated calls, in any order, are fast and use only O(1) extra
    memory per element.                                 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache: indices 0 … last-1 are stored */
    static seq_t *cache = NULL;
    static idx_t  last  = 0;                /* number of cached elements */

    /* base values for indices 0 … 5 */
    static const seq_t base[6] = { 1, 3, 1, 21, 1, 108 };

    /* Expand the cache to hold up to index n (inclusive) */
    if (last == 0 || last <= n)
    {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                         /* allocation failed */
        cache = tmp;
    }

    /* Ensure base values 0 … 5 are initialised if needed  */
    /* Compute the highest base index that must be set        */
    idx_t base_last_needed = (n < 5 ? (idx_t)(n + 1) : (idx_t)6);

    if (last < base_last_needed)
    {
        for (idx_t i = last; i < base_last_needed; ++i)
            cache[i] = base[i];
        last = base_last_needed;
    }

    /* Compute remaining terms using the recurrence  */
    if (last <= n)                      /* n ≥ 6 */
    {
        for (idx_t i = last; i <= n; ++i)
        {
            seq_t t = mul_small(cache[i - 2], 6);        /* 6 * a(i-2) */
            t -= mul_small(cache[i - 4], 6);            /* - 6 * a(i-4) */
            t += cache[i - 6];                          /* + a(i-6)  */
            cache[i] = t;
        }
        last = n + 1;
    }

    return cache[n];
}
