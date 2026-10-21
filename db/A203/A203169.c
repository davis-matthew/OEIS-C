/*
 * A203169  Sum of the fourth powers of the first n even-indexed Fibonacci
 *          numbers.
 * https://oeis.org/A203169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 56 a(n-1) - 440 a(n-2) + 770 a(n-3)
   - 440 a(n-4) + 56 a(n-5) - a(n-6),
   with a(0)=0, a(1)=1, a(2)=82, a(3)=4178,
   a(4)=198659, a(5)=9349284.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;   /* pointer to stored values */
    static idx_t  cache_len = 0;   /* number of values cached */

    /* initialise cache with first 6 terms the first time */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)82;
        cache[3] = (seq_t)4178;
        cache[4] = (seq_t)198659;
        cache[5] = (seq_t)9349284;
        cache_len = 6;
    }

    /* if already computed return the stored value */
    if (n < cache_len)
        return cache[n];

    /* ensure enough space */
    idx_t needed = n + 1;
    if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – return 0 */
        /* compute missing values via recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val =  ( (seq_t)56   * cache[i-1]
                         - (seq_t)440  * cache[i-2]
                         + (seq_t)770  * cache[i-3]
                         - (seq_t)440  * cache[i-4]
                         + (seq_t)56   * cache[i-5]
                         -            cache[i-6] );
            cache[i] = val;
        }
        /* update cached length */
        cache_len = needed;
    }

    return cache[n];
}
