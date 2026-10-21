/*
 * A222160  Number of (n+2) X 1 arrays of occupancy after each element moves
 *          up to +-2 places but not 0 and without 2-loops.
 * https://oeis.org/A222160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3) - 2*a(n-4) - 2*a(n-5) -
 *            a(n-6) Initial values: a(1)=1, a(2)=5, a(3)=17, a(4)=53,
 *            a(5)=159, a(6)=467
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0 – return 0 */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;     /* current capacity of cache */
    static idx_t   last  = 0;     /* largest n computed so far */

    /* return cached value if already computed */
    if (n <= last)
        return cache[n - 1];

    /* ensure the cache can hold at least n elements */
    size_t need = (size_t)n;
    if (need > cap) {
        size_t newcap = cap ? (cap << 1) : 16;
        if (newcap < need)
            newcap = need;
        seq_t *new_arr;

#ifdef __cplusplus
        /* C++: use new[]/delete[] for proper object construction */
        new_arr = new seq_t[newcap];
        if (cache) {
            for (size_t k = 0; k < cap; ++k)
                new_arr[k] = cache[k];   /* copy existing elements */
            delete[] cache;              /* destroy old array */
        }
#else
        /* C: use realloc (objects are trivially constructed) */
        new_arr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!new_arr)
            abort();                     /* out of memory */
#endif
        cache = new_arr;
        cap = newcap;
    }

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 6) {                      /* base values */
            switch ((int)i) {
                case 1: cache[0] = (seq_t)1;  break;
                case 2: cache[1] = (seq_t)5;  break;
                case 3: cache[2] = (seq_t)17; break;
                case 4: cache[3] = (seq_t)53; break;
                case 5: cache[4] = (seq_t)159; break;
                case 6: cache[5] = (seq_t)467; break;
            }
        } else {                            /* recurrence */
            seq_t t = (seq_t)3 * cache[i - 2] + cache[i - 3]
                       - (seq_t)3 * cache[i - 4]
                       - (seq_t)2 * cache[i - 5]
                       - (seq_t)2 * cache[i - 6]
                       - cache[i - 7];
            cache[i - 1] = t;
        }
    }

    last = n;
    return cache[n - 1];
}
