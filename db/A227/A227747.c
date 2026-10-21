/*
 * A227747  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having determinant equal to one.
 * https://oeis.org/A227747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A227747
 * Empirical recurrence:
 *   a(n) = a(n-1) + 4*a(n-2) + 8*a(n-3) + 4*a(n-4) + a(n-5) - a(n-6)
 * Initial terms:
 *   a(1)=4, a(2)=13, a(3)=40, a(4)=129, a(5)=409, a(6)=1300
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* bignum types might be > 64 bit; intermediate
       arithmetic uses seq_t only which is valid for the build. */

    if (n == 0) return (seq_t)0;          /* not used in the sequence (offset 1) */

    size_t sn = (size_t)n;                /* safe, idx_t is u64 */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 16;                   /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure: unlikely */

        /* store known values at indices 1..6 */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)40;
        cache[4] = (seq_t)129;
        cache[5] = (seq_t)409;
        cache[6] = (seq_t)1300;
        cache_size = 6;
    }

    /* grow cache if needed */
    if (sn > cache_size) {
        if (sn >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= sn) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute recurrence for missing terms */
        for (size_t k = cache_size + 1; k <= sn; ++k) {
            seq_t sum = cache[k - 1];
            sum += 4 * cache[k - 2];
            sum += 8 * cache[k - 3];
            sum += 4 * cache[k - 4];
            sum += cache[k - 5];
            sum -= cache[k - 6];
            cache[k] = sum;
        }
        cache_size = sn;
    }

    return cache[sn];
}
