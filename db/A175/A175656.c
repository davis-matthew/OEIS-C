/*
 * A175656  Eight bishops and one elephant on a 3 X 3 chessboard. G.f.:
 *          (1-3*x^2)/(1-3*x+4*x^3).
 * https://oeis.org/A175656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(n)=3*a(n-1)-4*a(n-3) (n≥3)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.size() <= n) {
        size_t old = cache.size();
        cache.resize(n + 1);
        for (size_t i = old; i < cache.size(); ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)3;
            else if (i == 2) cache[i] = (seq_t)6;
            else cache[i] = (seq_t)3 * cache[i-1] - (seq_t)4 * cache[i-3];
        }
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc((void*)cache, cache_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = old_len; i < cache_len; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)3;
            else if (i == 2) cache[i] = (seq_t)6;
            else cache[i] = (seq_t)3 * cache[i-1] - (seq_t)4 * cache[i-3];
        }
    }
    return cache[n];
#endif
}
