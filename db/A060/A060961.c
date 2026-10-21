/*
 * A060961  Number of compositions (ordered partitions) of n into 1's, 3's and
 *          5's.
 * https://oeis.org/A060961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) with a(0) = 1 and a(k) = 0 for k
 *            < 0
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 50, u64 n <= 99, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values (memoisation) */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t */
    static idx_t  len     = 0;      /* number of valid entries in cache */
    static idx_t  cap     = 0;      /* allocated capacity of cache */

    /* enlarge cache to hold index n */
    if (n >= len)
    {
        /* grow capacity (at least 16 or double until we can hold n) */
        idx_t new_cap = (cap == 0) ? 16 : cap;
        while (new_cap <= n) new_cap <<= 1;

        if (new_cap != cap)
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }

        /* initialise entries from current length up to n inclusive */
        for (idx_t i = len; i <= n; ++i)
        {
            seq_t val;
            if (i == 0)
            {
                val = (seq_t)1;
            }
            else
            {
                seq_t v1 = (i >= 1) ? cache[i-1] : (seq_t)0;
                seq_t v3 = (i >= 3) ? cache[i-3] : (seq_t)0;
                seq_t v5 = (i >= 5) ? cache[i-5] : (seq_t)0;
                val = v1 + v3 + v5;
            }
            cache[i] = val;
        }
        len = n + 1;
    }

    return cache[n];
}
