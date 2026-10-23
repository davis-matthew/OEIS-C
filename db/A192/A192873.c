/*
 * A192873  Coefficient of x in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) given in Comments.
 * https://oeis.org/A192873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4); a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=7, a(4)=18
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of previously computed terms   */
    static idx_t cache_size = 0;   /* number of entries currently stored   */

    /* make sure the cache is large enough for index n */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = n + 1;                /* grow to fit n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();                           /* allocation failed */
        cache = new_cache;

        /* fill new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)0;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)2;
            else if (i == 3)     cache[i] = (seq_t)7;
            else if (i == 4)     cache[i] = (seq_t)18;
            else {
                /* recurrence: a(i) = 3*a(i-1) - 3*a(i-3) + a(i-4) */
                seq_t val = cache[i-1];
                val *= (seq_t)3;
                val -= cache[i-3] * (seq_t)3;
                val += cache[i-4];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
