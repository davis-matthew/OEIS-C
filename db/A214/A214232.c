/*
 * A214232  Number of 0..7 colorings of an n X 3 array circular in the 3
 *          direction with new values 0..7 introduced in row major order.
 * https://oeis.org/A214232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 343*a(n-1) - 29965*a(n-2) + 856493*a(n-3) * -
 *            7269038*a(n-4) + 11346368*a(n-5); * Initial values: a(1)=1,
 *            a(2)=18, a(3)=1067, a(4)=133265, a(5)=24424881
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* handle only positive indices; for n==0 return 0 */
    if (n <= 1) return (n == 1 ? (seq_t)1 : (seq_t)0);

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* initialise cache with initial values */
    if (!cache) {
        cache_max = 5;
        cache = (seq_t *)malloc((cache_max + 1) * sizeof(seq_t));
        /* indexes 1..5 */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)1067;
        cache[4] = (seq_t)133265;
        cache[5] = (seq_t)24424881;
    }

    /* extend cache if needed */
    if (n > cache_max) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* memory allocation failed, return 0 as fallback */
        cache = new_cache;
        /* compute new terms using recurrence */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* positive part */
            seq_t pos = mul_small(cache[i - 1], 343U);
            pos    += mul_small(cache[i - 3], 856493U);
            pos    += mul_small(cache[i - 5], 11346368U);
            /* negative part */
            seq_t neg = mul_small(cache[i - 2], 29965U);
            neg    += mul_small(cache[i - 4], 7269038U);
            /* final term */
            cache[i] = pos - neg;  /* pos >= neg for all valid i */
        }
        cache_max = n;
    }

    return cache[n];
}
