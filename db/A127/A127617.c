/*
 * A127617  Number of walks from (0,0) to (n,n) in the region 0 <= x-y <= 3
 *          with the steps (1,0), (0, 1), (2,0) and (0,2).
 * https://oeis.org/A127617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0)=1, a(1)=1, a(2)=5, a(3)=22 a(n) = 3*a(n-1) + 5*a(n-2) +
 *            2*a(n-3) - a(n-4) (n >= 4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values         */
    static size_t  cap   = 0;        /* current allocated size (entries) */
    static size_t  last  = 0;        /* index of the highest computed value */

    /* ensure there is room for index n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = (cap == 0 ? 16 : cap * 2);
        if (newcap < needed)
            newcap = needed;
        if (cache == NULL)
            cache = (seq_t*)malloc(newcap * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* initialise base values on first use */
    if (last == 0) {
        cache[0] = (seq_t)1;
        if (cap > 1)
            cache[1] = (seq_t)1;
        if (cap > 2)
            cache[2] = (seq_t)5;
        if (cap > 3)
            cache[3] = (seq_t)22;
        last = 3;
    }

    /* compute missing values up to n (if any) */
    size_t target = (size_t)n;
    if (target > last) {
        for (size_t i = last + 1; i <= target; ++i) {
            cache[i] = (seq_t)3 * cache[i-1]
                     + (seq_t)5 * cache[i-2]
                     + (seq_t)2 * cache[i-3]
                     - cache[i-4];
        }
        last = target;
    }

    return cache[n];
}
