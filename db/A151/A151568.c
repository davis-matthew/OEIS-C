/*
 * A151568  a(0)=1, a(1)=1; a(2^i+j)=2*a(j)+a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; a(2^i + j) = 2*a(j) + a(j+1) for 0 ≤ j < 2^i
 * Range:     u8 n <= 61, u16 n <= 4087, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

#ifdef __cplusplus
#include <vector>
namespace {
    static std::vector<seq_t> cache_vec;
    static idx_t max_calculated = 0;

    static void ensure_cache(idx_t n) {
        if (cache_vec.size() <= n) cache_vec.resize(n + 1);
    }
}
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t max_calculated = 0;
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    if (cache_vec.empty()) {
        cache_vec.resize(2);
        cache_vec[0] = 1;
        cache_vec[1] = 1;
        max_calculated = 1;
    }

    if (n <= max_calculated) return cache_vec[n];

    ensure_cache(n);
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        idx_t p = ((idx_t)1) << floor_log2_uint64(i);
        idx_t j = i - p;
        cache_vec[i] = 2 * cache_vec[j] + cache_vec[j + 1];
    }
    max_calculated = n;
    return cache_vec[n];
#else
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 1;
        max_calculated = 1;
    }

    if (n <= max_calculated) return cache[n];

    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        idx_t p = ((idx_t)1) << floor_log2_uint64(i);
        idx_t j = i - p;
        cache[i] = 2 * cache[j] + cache[j + 1];
    }
    max_calculated = n;
    return cache[n];
#endif
}
