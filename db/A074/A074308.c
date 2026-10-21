/*
 * A074308  Inverse of permutation in A074307.
 * https://oeis.org/A074308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) + a(n-16) - a(n-24) Initial values: a(0)=0,
 *            a(1)=3, a(2)=1, a(3)=2, a(4)=9, a(5)=5, a(6)=8, a(7)=6, a(8)=18,
 *            a(9)=12, a(10)=10, a(11)=4, a(12)=27, a(13)=7, a(14)=17,
 *            a(15)=15, a(16)=36, a(17)=21, a(18)=19, a(19)=11, a(20)=45,
 *            a(21)=14, a(22)=26, a(23)=24
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* capacity of allocated array   */
    static size_t   sz    = 0;   /* number of values already computed */

    /* Initialisation: allocate space and fill the first 24 terms */
    if (cache == NULL)
    {
        cap = 32;                         /* start with a reasonable size */
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        /* Initial values of the sequence */
        const seq_t base[24] = {
            0, 3, 1, 2,
            9, 5, 8, 6,
            18, 12, 10, 4,
            27, 7, 17, 15,
            36, 21, 19, 11,
            45, 14, 26, 24
        };
        for (size_t i = 0; i < 24; ++i)
        {
            cache[i] = base[i];
        }
        sz = 24;
    }

    /* Grow the cache and compute further terms if necessary */
    if ((size_t)n >= sz)
    {
        /* Ensure enough capacity */
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        if (new_cap > cap)
        {
#ifdef __cplusplus
            /* Allocate new array and copy old values */
            seq_t *new_ptr = new seq_t[new_cap];
            for (size_t i = 0; i < sz; ++i) new_ptr[i] = cache[i];
            delete[] cache;
            cache = new_ptr;
#else
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_ptr;
#endif
            cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (size_t i = sz; i <= (size_t)n; ++i)
        {
            cache[i] = cache[i - 8] + cache[i - 16] - cache[i - 24];
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
