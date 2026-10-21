/*
 * A075796  Numbers k such that 5*k^2 + 5 is a square.
 * https://oeis.org/A075796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2); a(1)=2, a(2)=38
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* unused, for safety */

#if defined(__cplusplus)
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(2);
        cache.push_back((seq_t)2);
        cache.push_back((seq_t)38);
    }
    while (cache.size() < n) {
        seq_t val = (seq_t)18 * cache[cache.size() - 1] - cache[cache.size() - 2];
        cache.push_back(val);
    }
    return cache[n - 1];
#else
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed */
        cache_size = 2;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)38;
    } else if (cache_size < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_size; i < n; ++i) {
            cache[i] = (seq_t)18 * cache[i - 1] - cache[i - 2];
        }
        cache_size = n;
    }
    return cache[n - 1];
#endif
}
