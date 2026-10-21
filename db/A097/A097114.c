/*
 * A097114  Expansion of (1 + 8x - 42x^2 - 392x^3)/(1 - 99x^2 + 2450x^4).
 * https://oeis.org/A097114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=57, a(3)=400,
   a(n)=99*a(n-2)-2450*a(n-4)  for n≥4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0, sz = 0;

    if ((idx_t)n >= sz)
    {
        /* Ensure capacity */
        idx_t need = n + 1;
        if (cap < need)
        {
            idx_t new_cap = cap ? cap << 1 : 4;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }
        /* Compute values up to n */
        for (idx_t i = sz; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)8;
            else if (i == 2)
                cache[2] = (seq_t)57;
            else if (i == 3)
                cache[3] = (seq_t)400;
            else
                cache[i] = (seq_t)99 * cache[i-2] -
                           (seq_t)2450 * cache[i-4];
        }
        sz = n + 1;
    }
    return cache[n];
}
