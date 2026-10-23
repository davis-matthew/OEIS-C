/*
 * A008336  a(n+1) = a(n)/n if n|a(n) else a(n)*n, a(1) = 1.
 * https://oeis.org/A008336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look, changed
 * Formula:   a(1) = 1; a(n+1) = a(n)/n if n | a(n), else a(n)*n otherwise
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 57, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
seq_t a(idx_t n) {
    static std::vector<seq_t> cache = {seq_t(1)};  // cache[0] == a(1)
    if (n == 0) return seq_t(0);                  // undefined, just safe return
    if (n > cache.size()) {
        for (idx_t k = cache.size() + 1; k <= n; ++k) {
            seq_t prev = cache.back();
            idx_t prev_n = k - 1;
            if (prev % static_cast<seq_t>(prev_n) == 0)
                cache.push_back(prev / static_cast<seq_t>(prev_n));
            else
                cache.push_back(prev * static_cast<seq_t>(prev_n));
        }
    }
    return cache[n - 1];
}
#else
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  // undefined, safe return
    if (n > cache_len) {
        size_t new_cap = (cache_len == 0) ? 1 : cache_len * 2;
        if ((size_t)n > new_cap) new_cap = (size_t)n;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }  // allocation failed
            cache = tmp;
            cache_cap = new_cap;
        }
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        for (size_t k = cache_len + 1; k <= n; ++k) {
            seq_t prev = cache[cache_len - 1];
            idx_t prev_n = k - 1;
            if (prev % (seq_t)prev_n == 0)
                cache[cache_len++] = prev / (seq_t)prev_n;
            else
                cache[cache_len++] = prev * (seq_t)prev_n;
        }
    }
    return cache[n - 1];
}
#endif
