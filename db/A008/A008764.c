/*
 * A008764  Number of 3 X 3 symmetric stochastic matrices under row and column
 *          permutations.
 * https://oeis.org/A008764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10) for n>=10.
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=8,
 *            a(6)=12, a(7)=16, a(8)=21, a(9)=27
 * Range:     u8 n <= 22, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
#endif

seq_t a(idx_t n)
{
    const seq_t base[10] = {1,1,2,4,6,8,12,16,21,27};

#ifdef __cplusplus
    size_t old_len = cache.size();
    if (old_len < (size_t)n + 1) cache.resize(n + 1);
    if (old_len < 10) {
        for (size_t i = old_len; i < 10 && i <= (size_t)n; ++i)
            cache[i] = base[i];
        old_len = 10;
    }
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-2]
                 - (seq_t)2 * cache[i-5] + cache[i-8]
                 + cache[i-9] - cache[i-10];
    }
    return cache[(size_t)n];
#else
    size_t prev_len = cache_len;
    if (prev_len < (size_t)n + 1) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }
    if (prev_len < 10) {
        for (size_t i = prev_len; i < 10 && i <= (size_t)n; ++i)
            cache[i] = base[i];
        prev_len = 10;
    }
    for (size_t i = prev_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-2]
                 - (seq_t)2 * cache[i-5] + cache[i-8]
                 + cache[i-9] - cache[i-10];
    }
    return cache[(size_t)n];
#endif
}
