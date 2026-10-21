/*
 * A054041  Sum of a(n) terms of 1/k^(1/3) first exceeds n.
 * https://oeis.org/A054041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A054041:  a(n) = smallest m such that 
   S(m) = sum_{k=1}^{m} 1 / k^(1/3)  >  n .
   The function maintains a cache so that repeated calls, in any order,
   are fast.  The calculation is performed incrementally using a
   long double for the cumulative sum; the individual terms are
   generated iteratively.
*/
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;        /* cached a(n) values */
    static size_t cache_size = 0;      /* allocated size of cache   */
    static idx_t cur_m = 0;            /* last index m processed   */
    static long double sum = 0.0L;     /* cumulative sum S(m)      */
    static idx_t next_n = 0;           /* next n whose a(n) is not yet known */

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 64;
        while (new_size <= (size_t)n) new_size <<= 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        /* if realloc fails we simply abort */
        cache_size = new_size;
    }

    /* compute until we know a[0..n] */
    while (next_n <= n) {
        /* advance to next term */
        cur_m++;
        /* add the term 1 / cur_m^(1/3) */
        sum += pow((long double)cur_m, -1.0L/3.0L);

        /* fill cache for all n crossed by this increment */
        while (next_n <= n && sum > (long double)next_n) {
            cache[next_n] = (seq_t)cur_m;
            next_n++;
        }
    }

    return cache[n];
}
