/*
 * A022026  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(2,15).
 * https://oeis.org/A022026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=15, a(n+2)=8*a(n+1)-4*a(n) for n≥0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached terms */
    static size_t cache_len = 0;    /* number of elements stored */

    /* Need at least two slots to store a(0) and a(1) */
    size_t needed = (size_t)n + 1;
    if (needed < 2) needed = 2;

    /* Expand cache if required */
    if (needed > cache_len)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – return a sentinel */
        cache = tmp;

        /* Initialise first two values if this is the first allocation */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)15;
            cache_len = 2;
        }
        else if (cache_len < 2)  /* safety for tricky malloc growth */
        {
            cache[1] = (seq_t)15;
            cache_len = 2;
        }

        /* Compute missing terms using the linear recurrence */
        for (size_t i = cache_len; i < needed; ++i)
        {
            seq_t t1 = cache[i - 1] * (seq_t)8;
            seq_t t2 = cache[i - 2] * (seq_t)4;
            cache[i] = t1 - t2;
        }
        cache_len = needed;
    }

    return cache[n];
}
