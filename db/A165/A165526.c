/*
 * A165526  Number of permutations of length n which avoid the patterns 4312
 *          and 1324.
 * https://oeis.org/A165526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A165526
   a(n) = 17*a(n-1) - 128*a(n-2) + 561*a(n-3) - 1581*a(n-4) + 2984*a(n-5)
          - 3804*a(n-6) + 3216*a(n-7) - 1712*a(n-8) + 512*a(n-9) - 64*a(n-10)
          (for n > 10;  a(10) = this sum + 4)
   Initial terms: a(0)=1, a(1)=1, a(2)=2, a(3)=6,
   a(4)=22, a(5)=86, a(6)=335, a(7)=1266, a(8)=4598, a(9)=16016
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Lazy memoised array for previously computed terms.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;       /* number of entries stored [0, cache_size)  */
    static size_t cache_cap = 0;        /* current allocation size                  */

    /* Grow the cache to at least n+1 entries.  */
    if (n >= cache_size) {
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
#else
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
            if (cache) {
#ifdef __cplusplus
                for (size_t i = 0; i < cache_size; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
#else
                memcpy(tmp, cache, cache_size * sizeof(seq_t));
                free(cache);
#endif
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initialise the first ten terms if not already done */
        if (cache_size < 10) {
            static const seq_t init[10] = {
                (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,
                (seq_t)22, (seq_t)86, (seq_t)335, (seq_t)1266,
                (seq_t)4598, (seq_t)16016
            };
            for (size_t i = cache_size; i < 10 && i < needed; ++i)
                cache[i] = init[i];
        }

        /* Compute missing values up to n.  */
        for (size_t i = cache_size; i < needed; ++i) {
            if (i < 10) continue;  /* already initialised */
            /* Positive contributions */
            seq_t pos = (seq_t)17 * cache[i-1] +
                        (seq_t)561 * cache[i-3] +
                        (seq_t)2984 * cache[i-5] +
                        (seq_t)3216 * cache[i-7] +
                        (seq_t)512  * cache[i-9];
            /* Negative contributions */
            seq_t neg = (seq_t)128 * cache[i-2] +
                        (seq_t)1581 * cache[i-4] +
                        (seq_t)3804 * cache[i-6] +
                        (seq_t)1712 * cache[i-8] +
                        (seq_t)64  * cache[i-10];

            seq_t val = pos - neg;
            if (i == 10) val += (seq_t)4;   /* the constant in the numerator */
            cache[i] = val;
        }
        cache_size = needed;
    }

    return cache[n];
}
