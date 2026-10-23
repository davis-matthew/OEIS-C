/*
 * A025994  Expansion of 1/((1-2*x)*(1-5*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A025994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(n+1)-2^(n+1))/3 + 17*a(n-1) - 70*a(n-2) with initial
 *            values a(0)=1, a(1)=24
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;        /* number of elements currently stored */

    if (n >= len)                   /* need to grow the cache */
    {
        idx_t old_len = len;         /* keep the previous length */
        idx_t new_len = n + 1;

        /* realloc (cast needed for C++ compilation) */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                /* memory allocation failed */
        cache = tmp;
        len   = new_len;

        /* initialise the base cases if they haven't been set yet */
        if (old_len == 0)
        {
            cache[0] = (seq_t)1;
            if (new_len > 1)
                cache[1] = (seq_t)24;
        }
        else if (old_len == 1)
        {
            cache[1] = (seq_t)24;
        }

        /* compute starting point for the new terms */
        idx_t start = (old_len > 2) ? old_len : (idx_t)2;

        /* compute powers 5^start and 2^start */
        seq_t p5 = (seq_t)1;
        seq_t p2 = (seq_t)1;
        for (idx_t i = 0; i < start; ++i)
        {
            p5 *= (seq_t)5;
            p2 *= (seq_t)2;
        }

        /* fill in the cache from 'start' up to 'n' */
        for (idx_t i = start; i <= n; ++i)
        {
            /* 5^(i+1) and 2^(i+1) */
            seq_t term5 = p5 * (seq_t)5;    /* 5^(i+1) */
            seq_t term2 = p2 * (seq_t)2;    /* 2^(i+1) */

            /* update the powers for the next iteration */
            p5 = term5;
            p2 = term2;

            /* recurrence part: (5^(i+1)-2^(i+1))/3 + 17*a(i-1) - 70*a(i-2) */
            seq_t val = (term5 - term2) / (seq_t)3;
            val += cache[i-1] * (seq_t)17;
            val -= cache[i-2] * (seq_t)70;

            cache[i] = val;
        }
    }

    return cache[n];
}
