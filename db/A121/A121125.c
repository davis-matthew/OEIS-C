/*
 * A121125  Unbranched a-4-catapolynonagons (see Brunvoll reference for
 *          precise definition).
 * https://oeis.org/A121125
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence derived from the rational generating function a(n) =
 *            18*a(n-1) - 96*a(n-2) + 1260*a(n-4) - 1944*a(n-5) - 3888*a(n-6)
 *            + 7776*a(n-7) - b(n) where b(0) = b(1) = 0 b(2) = -1 b(3) = 14
 *            b(4) = -43 b(5) = -165 b(6) = 816 b(7) = 504 b(8) = -3474 b(9) =
 *            -486 b(10)= 2700 b(11)= 1844 and b(n)=0 f...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for indices 0 … 11  (values for index < 2 are 0) */
    static const int64_t B[12] = {
        0, 0,
        -1,   14,  -43, -165,   816,   504,
        -3474,-486, 2700, 1844
    };

    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries in cache (>=0) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 12;                     /* indices 0 … 11 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* memory failure */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 4;
        cache[4] = 19;
        cache[5] = 123;
        cache[6] = 834;
        cache[7] = 5796;
        cache[8] = 40014;
        cache[9] = 274590;
        cache[10] = 1867320;
        cache[11] = 12600360;
    }

    if (n < cache_size)            /* already known */
        return cache[n];

    /* Grow cache if required */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory failure */
        cache = tmp;

        /* Compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Positive part of the recurrence */
            seq_t pos = cache[i-1] * (seq_t)18
                      + cache[i-4] * (seq_t)1260
                      + cache[i-7] * (seq_t)7776;

            /* Negative part of the recurrence */
            seq_t neg = cache[i-2] * (seq_t)96
                      + cache[i-5] * (seq_t)1944
                      + cache[i-6] * (seq_t)3888;

            /* Add the (possibly negative) b(i) term */
            int64_t bi = (i < 12 ? B[i] : 0);
            if (bi >= 0)
                neg += (seq_t)bi;      /* subtract b(i) */
            else
                pos += (seq_t)(-bi);   /* subtract negative b(i) */

            /* Final value = pos - neg */
            cache[i] = pos - neg;        /* pos > neg for n ≥ 12 */
        }
        cache_size = new_size;
    }

    return cache[n];
}
