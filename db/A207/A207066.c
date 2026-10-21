/*
 * A207066  Number of n X 6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -21*a(n-2) +35*a(n-3) -35*a(n-4) * +21*a(n-5)
 *            -7*a(n-6) + a(n-7) for n > 7 * a(1)=21, a(2)=441, a(3)=2499,
 *            a(4)=8799, * a(5)=23856, a(6)=54684, a(7)=111426
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static size_t cache_len   = 0;   /* number of cached terms   */
    static size_t cache_cap   = 0;   /* allocated capacity      */
    static seq_t *cache       = NULL;/* cached sequence terms    */

    /* recurrence coefficients; coefficient[0] multiplies a(n-1)  */
    const int64_t coeff[7] = { 7, -21, 35, -35, 21, -7, 1 };

    /* initialise cache with the first 7 values */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)21;     /* a(1) */
        cache[1] = (seq_t)441;    /* a(2) */
        cache[2] = (seq_t)2499;   /* a(3) */
        cache[3] = (seq_t)8799;   /* a(4) */
        cache[4] = (seq_t)23856;  /* a(5) */
        cache[5] = (seq_t)54684;  /* a(6) */
        cache[6] = (seq_t)111426; /* a(7) */
        cache_len = 7;
    }

    /* return immediately if the requested value is already cached */
    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* extend the cache until we have computed a(n) */
    for (size_t cur = cache_len; cur < (size_t)n; ++cur) {
        /* enlarge storage if necessary */
        if (cur + 1 > cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* compute a(cur+1) using the recurrence.
         * cache[(cur-k)-1] refers to a((cur-k)+1) = a(cur-k+1) = a(cur-k+1) */
        __int128 sum = 0;
        for (int k = 0; k < 7; ++k) {
            sum += coeff[k] * (__int128)cache[cur - k - 1];
        }
        cache[cur] = (seq_t)sum;
    }

    return cache[n-1];
}
