/*
 * A228778  a(n) = 2^Fibonacci(n) + 1.
 * https://oeis.org/A228778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^Fibonacci(n) + 1 Recurrence: a(n+2) = a(n+1)*a(n) -
 *            a(n) - a(n+1) + 2, a(0) = 2, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
/* static cache for C++ builds */
static std::vector<seq_t> cache_v;
#else
/* static cache for C builds */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    size_t required = (size_t)n + 1;
    size_t old_len = cache_v.size();
    if (old_len < required) {
        cache_v.resize(required);
        if (old_len == 0) {
            cache_v[0] = (seq_t)2;
            old_len = 1;
        }
        if (old_len == 1) {
            cache_v[1] = (seq_t)3;
            old_len = 2;
        }
        for (idx_t i = (idx_t)old_len; i <= n; ++i) {
            seq_t a_prev  = cache_v[i - 1];
            seq_t a_pprev = cache_v[i - 2];
            seq_t tmp = a_prev * a_pprev;
            tmp = tmp - a_prev;
            tmp = tmp - a_pprev;
            tmp = tmp + (seq_t)2;
            cache_v[(size_t)i] = tmp;
        }
    }
    return cache_v[(size_t)n];
#else
    size_t required = (size_t)n + 1;
    if (cache_len < required) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 4;
        while (new_cap < required) new_cap <<= 1;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;

        if (cache_len == 0) {
            cache[0] = (seq_t)2;
            cache_len = 1;
        }
        if (cache_len == 1 && required > 1) {
            cache[1] = (seq_t)3;
            cache_len = 2;
        }
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t a_prev  = cache[i - 1];
            seq_t a_pprev = cache[i - 2];
            seq_t tmp = a_prev * a_pprev;
            tmp = tmp - a_prev;
            tmp = tmp - a_pprev;
            tmp = tmp + (seq_t)2;
            cache[(size_t)i] = tmp;
        }
        cache_len = required;
    }
    return cache[(size_t)n];
#endif
}
