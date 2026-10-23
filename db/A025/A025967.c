/*
 * A025967  Expansion of 1/((1-2*x)*(1-4*x)*(1-6*x)*(1-9*x)).
 * https://oeis.org/A025967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 152*a(n-2) + 444*a(n-3) - 432*a(n-4), a(0)=1,
 *            a(1)=21, a(2)=289, a(3)=3321
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Memoised storage of computed terms */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.assign(4, 0);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)289;
        cache[3] = (seq_t)3321;
    }
    typedef std::size_t size_t;
    size_t sz = cache.size();
    if ((idx_t)sz <= n) {
        cache.resize(n + 1);
        for (idx_t i = (idx_t)sz; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = (seq_t)21 * a1
                     - (seq_t)152 * a2
                     + (seq_t)444 * a3
                     - (seq_t)432 * a4;
        }
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)289;
        cache[3] = (seq_t)3321;
    }
    if (needed > cache_cap) {
        cache_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */ exit(1); }
        cache = tmp;
    }
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = (seq_t)21 * a1
                 - (seq_t)152 * a2
                 + (seq_t)444 * a3
                 - (seq_t)432 * a4;
    }
    return cache[n];
#endif
}
