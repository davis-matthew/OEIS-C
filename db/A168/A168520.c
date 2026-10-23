/*
 * A168520  a(n) = 98*a(n-1) - a(n-2); a(1) = 0, a(2) = 10.
 * https://oeis.org/A168520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; a(2) = 10; a(n) = 98*a(n-1) - a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation uses std::vector for automatic construction. */
    static std::vector<seq_t> cache;
    static idx_t max_n = 0;

    if (n == 1) return seq_t(0);
    if (n == 2) return seq_t(10);

    if (max_n < 2) {
        cache.resize(3);
        cache[1] = seq_t(0);
        cache[2] = seq_t(10);
        max_n = 2;
    }

    if (n > max_n) {
        if (cache.size() - 1 < n) cache.resize(n + 1);
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = (seq_t)98 * cache[i-1] - cache[i-2];
        }
        max_n = n;
    }

    return cache[n];
#else
    /* C implementation with manual memory management. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* highest index currently stored */

    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;

    /* initial allocation for base terms */
    if (cache_size < 2) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* unlikely but safe */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)10;
        cache_size = 2;
    }

    /* grow cache as necessary */
    if (n > cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* unlikely but safe */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)98 * cache[i-1] - cache[i-2];
        }
        cache_size = n;
    }

    return cache[n];
#endif
}
