/*
 * A021972  Decimal expansion of 1/968.
 * https://oeis.org/A021972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-11) + a(n-12) for n > 14, with initial terms
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=3, a(5)=3, a(6)=0, a(7)=5,
 *            a(8)=7, a(9)=8, a(10)=5, a(11)=1, a(12)=2, a(13)=3, a(14)=9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stddef.h>

static const seq_t _a_init[15] = {
    0, 0, 1, 0, 3,
    3, 0, 5, 7, 8,
    5, 1, 2, 3, 9
};

seq_t a(idx_t n) {
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(15);
        for (size_t i = 0; i < 15; ++i) cache[i] = _a_init[i];
    }
    size_t target = (size_t)n;
    if (target < cache.size()) return cache[target];
    // Compute missing terms up to target
    for (size_t i = cache.size(); i <= target; ++i) {
        long long val = (long long)cache[i-1] - (long long)cache[i-11] + (long long)cache[i-12];
        cache.push_back((seq_t)val);
    }
    return cache[target];
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t next = 0; /* next index to compute */
    if (cache == NULL) {
        /* initial allocation with first 15 terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 15; ++i) cache[i] = _a_init[i];
        next = 15;
    }
    size_t target = (size_t)n;
    if (target < next) return cache[target];
    /* grow cache if necessary */
    while (next <= target) {
        if (next >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= next) new_cap = next + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* out of memory, fall back to static value if within bounds */
                if (next <= target) {
                    /* compute remaining terms locally without storage */
                    for (size_t i = next; i <= target; ++i) {
                        long long val = (long long)cache[i-1] - (long long)cache[i-11] + (long long)cache[i-12];
                        if (i == target) return (seq_t)val;
                    }
                }
                cache = tmp; /* may be NULL */
                cache_cap = new_cap;
            } else {
                cache = tmp;
                cache_cap = new_cap;
            }
        }
        long long val = (long long)cache[next-1] - (long long)cache[next-11] + (long long)cache[next-12];
        cache[next] = (seq_t)val;
        ++next;
    }
    return cache[target];
#endif
}
