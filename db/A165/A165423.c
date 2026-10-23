/*
 * A165423  a(1) = 1, a(2) = 5, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=5, a(3)=5, a(n)=a(n-1)^2 for n>=4
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A165423 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* Undefined for n<1 */

    /* Cache previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries + 1 */

    /* Ensure cache can hold index n */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;   /* +1 so that cache[n] is valid */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;   /* Allocation failure – return zero as fallback */

        /* Initialize new entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i)
        {
            if (i == 1)
                new_cache[i] = 1;
            else if (i == 2)
                new_cache[i] = 5;
            else if (i == 3)
                new_cache[i] = 5;
            else
            {
                /* square the previous term */
                seq_t prev = new_cache[i - 1];
                new_cache[i] = prev * prev;
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
