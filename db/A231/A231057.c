/*
 * A231057  Number of n X 2 0..3 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 4, no adjacent elements equal, and upper left element zero.
 * https://oeis.org/A231057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + a(n-3) with a(1)=0, a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms cached, offset 1 */

    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but defined as 0 for safety */

    /* extend cache if necessary */
    if (cache_size < n)
    {
        size_t new_size = (size_t)n;
        seq_t *tmp = mem_new(seq_t, new_size);
        if (!tmp)
            exit(1);   /* allocation failed */

        /* copy existing terms */
        if (cache)
            mem_copy(tmp, cache, (size_t)cache_size * sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = cache_size; i < n; ++i)
        {
            if (i == 0)            /* a(1) */
                tmp[i] = (seq_t)0;
            else if (i == 1)       /* a(2) */
                tmp[i] = (seq_t)2;
            else if (i == 2)       /* a(3) */
                tmp[i] = (seq_t)6;
            else                   /* recurrence */
                tmp[i] = (seq_t)3 * tmp[i-1] + tmp[i-2] + tmp[i-3];
        }

        /* free old cache */
        mem_free(cache);
        cache = tmp;
        cache_size = n;
    }

    return cache[n-1];
}
