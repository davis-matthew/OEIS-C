/*
 * A038196  3-wave sequence starting with 1, 1, 1.
 * https://oeis.org/A038196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) if n is odd, a(n) = a(n-1) + a(n-4) if n
 *            is even Initial values: a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 56, u64 n <= 110,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache array */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* largest index that has been computed */
    static size_t cap = 0;        /* allocated capacity */

    /* initial allocation and seed values */
    if (cache == NULL)
    {
        cap = 8;                 /* small initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed, unlikely */

        /* set the known initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_cached = 2;
    }

    /* ensure the cache is large enough */
    if ((idx_t)(cap) <= n)
    {
        size_t new_cap = cap;
        while ((size_t)(new_cap) <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* reallocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute values up to n if needed */
    if (max_cached < n)
    {
        for (idx_t i = max_cached + 1; i <= n; ++i)
        {
            if ((i & 1) == 1)          /* i is odd */
            {
                cache[i] = cache[i - 1] + cache[i - 2];
            }
            else                       /* i is even */
            {
                /* for i==0,1,2 already handled; for i==4 and higher */
                cache[i] = cache[i - 1] + cache[i - 4];
            }
        }
        max_cached = n;
    }

    return cache[n];
}
