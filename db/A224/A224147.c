/*
 * A224147  Number of 3 X n 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (n>7) initial values: a(1)=4, a(2)=16,
 *            a(3)=49, a(4)=124, a(5)=275, a(6)=554, a(7)=1037
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 119, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;

    if (n < 1)                    /* offset is 1; undefined for n<1 */
        return 0;

    /* already computed? */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;            /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* base values for n = 1..7 */
    static const seq_t base[7] = {4, 16, 49, 124, 275, 554, 1037};

    /* compute successive terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 7) {
            val = base[i - 1];
        } else {
            val = (seq_t)7   * cache[i - 2]
                - (seq_t)21  * cache[i - 3]
                + (seq_t)35  * cache[i - 4]
                - (seq_t)35  * cache[i - 5]
                + (seq_t)21  * cache[i - 6]
                - (seq_t)7   * cache[i - 7]
                + cache[i - 8];
        }
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
