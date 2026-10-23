/*
 * A017846  Expansion of
 *          1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15).
 * https://oeis.org/A017846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)
 *            for n>14, with initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0,
 *            a(5)=1, a(6)=1, a(7)=1, a(8)=1, a(9)=1, a(10)=2, a(11)=3,
 *            a(12)=4, a(13)=5, a(14)=6, a(15)=8
 * Range:     u8 n <= 27, u16 n <= 47, u32 n <= 88, u64 n <= 168,
 *            u128 n <= 329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static bool initialized = false;

    /* Convert the unsigned index to a size_t for array access */
    size_t idx = (size_t)n;

    if (!initialized)
    {
        /* Allocate space for indices 0..15 initially */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure: return 0 as safe default */

        /* Base values */
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)0;
        cache[5] = cache[6] = cache[7] = cache[8] = cache[9] = (seq_t)1;
        cache[10] = (seq_t)2;
        cache[11] = (seq_t)3;
        cache[12] = (seq_t)4;
        cache[13] = (seq_t)5;
        cache[14] = (seq_t)6;
        cache[15] = (seq_t)8;

        cache_len = 16;       /* computed values up to index 15 inclusive */
        initialized = true;
    }

    /* Grow the cache if needed */
    if (idx >= cache_len)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= idx)
            new_cap <<= 1;           /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute the new entries using the recurrence */
        for (size_t i = cache_len; i <= idx; ++i)
        {
            seq_t sum = 0;
            for (int k = 5; k <= 15; ++k)
                if (i >= (size_t)k)
                    sum += cache[i - k];
            cache[i] = sum;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
