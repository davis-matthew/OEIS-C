/*
 * A161370  a(n) = 2*A010844(n) + 1.
 * https://oeis.org/A161370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; a(n)=2*n*a(n-1)-(2*n-3) for n≥1 */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    size_t sz = cache.size();
    if (sz <= static_cast<size_t>(n)) {
        cache.resize(n + 1);
        for (idx_t k = static_cast<idx_t>(sz); k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = 3;
            } else {
                seq_t prev = cache[k - 1];
                seq_t mult = (seq_t)k * 2;
                val = prev * mult - (mult - 3);
            }
            cache[k] = val;
        }
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    if (cache_len <= n) {
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = 3;
            } else {
                seq_t prev = cache[k - 1];
                seq_t mult = (seq_t)k * 2;
                val = prev * mult - (mult - 3);
            }
            if (k >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                while (new_cap <= (size_t)k) new_cap *= 2;
                cache = realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[k] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
#endif
}
