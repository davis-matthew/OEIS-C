/*
 * A217631  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..1 n X 2 array.
 * https://oeis.org/A217631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=6, a(3)=16, a(n)=a(n-1)+3*a(n-2)+2*a(n-3)
 *            for n>=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cache_len = 0;     /* number of valid entries in cache */
    static size_t cache_cap = 0;     /* current allocated capacity */

    /* extend cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;

        /* grow capacity if necessary */
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap < new_len) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)          cache[i] = 0;
            else if (i == 1)     cache[i] = 2;
            else if (i == 2)     cache[i] = 6;
            else if (i == 3)     cache[i] = 16;
            else                 cache[i] = cache[i-1] + 3*cache[i-2] + 2*cache[i-3];
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
