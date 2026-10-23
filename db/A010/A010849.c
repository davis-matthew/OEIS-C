/*
 * A010849  Let S(x,y) = number of lattice paths from (0,0) to (x,y) that use
 *          the step set { (0,1), (1,0), (2,0), (3,0), ....} and never pass
 *          below y = x. Sequence gives S(n-3,n).
 * https://oeis.org/A010849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=18 For n>=3: n*(n+4)*a[n] =
 *            (7*n^2+16*n-3)*a[n-1] - (7*n^2-2*n-12)*a[n-2] +
 *            (n-3)*(n+1)*a[n-3]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with base terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1U;
        cache[1] = (seq_t)4U;
        cache[2] = (seq_t)18U;
    }

    if (n < cache_len)
        return cache[n];

    /* enlarge cache to hold all terms up to n */
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    /* compute missing terms */
#ifdef __cplusplus
    if (std::is_integral<seq_t>::value) {
        for (idx_t i = cache_len; i <= n; ++i) {
            __uint128_t ii = i;
            __uint128_t t1 = 7 * ii * ii + 16 * ii - 3;
            __uint128_t t2 = 7 * ii * ii - 2 * ii - 12;
            __uint128_t t3 = (ii - 3) * (ii + 1);
            __uint128_t num = t1 * ( __uint128_t)cache[i - 1]
                            - t2 * ( __uint128_t)cache[i - 2]
                            + t3 * ( __uint128_t)cache[i - 3];
            __uint128_t den = ii * (ii + 4);
            cache[i] = (seq_t)(num / den);
        }
    } else {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t ii = (seq_t)i;
            seq_t t1 = (seq_t)7 * ii * ii + (seq_t)16 * ii - (seq_t)3;
            seq_t t2 = (seq_t)7 * ii * ii - (seq_t)2 * ii - (seq_t)12;
            seq_t t3 = (ii - (seq_t)3) * (ii + (seq_t)1);
            seq_t num = t1 * cache[i - 1] - t2 * cache[i - 2] + t3 * cache[i - 3];
            seq_t den = ii * (ii + (seq_t)4);
            cache[i] = num / den;
        }
    }
#else
    for (idx_t i = cache_len; i <= n; ++i) {
        __uint128_t ii = i;
        __uint128_t t1 = 7 * ii * ii + 16 * ii - 3;
        __uint128_t t2 = 7 * ii * ii - 2 * ii - 12;
        __uint128_t t3 = (ii - 3) * (ii + 1);
        __uint128_t num = t1 * ( __uint128_t)cache[i - 1]
                        - t2 * ( __uint128_t)cache[i - 2]
                        + t3 * ( __uint128_t)cache[i - 3];
        __uint128_t den = ii * (ii + 4);
        cache[i] = (seq_t)(num / den);
    }
#endif

    cache_len = n + 1;
    return cache[n];
}
