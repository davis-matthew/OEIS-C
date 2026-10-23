/*
 * A042675  Denominators of continued fraction convergents to sqrt(867).
 * https://oeis.org/A042675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 140452*a(n-6) - a(n-12) a(0)=1, a(1)=2, a(2)=9, a(3)=263,
 *            a(4)=1061, a(5)=2385, a(6)=139391, a(7)=281167, a(8)=1264059,
 *            a(9)=36938878, a(10)=149019571, a(11)=334978020
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cstdlib>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

seq_t a(idx_t n)
{
    /* initial 12 terms, needed for all implementations */
    static const seq_t init[12] = {
        1, 2, 9, 263, 1061, 2385,
        139391, 281167, 1264059, 36938878,
        149019571, 334978020
    };

#ifdef __cplusplus
    /* C++ implementation using std::vector for proper construction/destruction */
    static std::vector<seq_t> cache;
    size_t sz = cache.size();
    if (n < sz)
        return cache[n];

    if (sz == 0) { /* ensure initial terms are present */
        size_t i;
        for (i = 0; i < 12 && i <= n; ++i)
            cache.push_back(init[i]);
        sz = cache.size();
    }
    while (sz <= (size_t)n) {
        if (sz < 12)
            cache.push_back(init[sz]);
        else
            cache.push_back((seq_t)140452 * cache[sz - 6] - cache[sz - 12]);
        ++sz;
    }
    return cache[n];
#else
    /* C implementation using realloc (seq_t must be a POD type) */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static size_t cache_cap  = 0;

    if (n >= cache_size) {
        /* grow capacity if needed */
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();          /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;

        /* fill missing values up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 12)
                cache[i] = init[i];
            else
                cache[i] = (seq_t)140452 * cache[i - 6] - cache[i - 12];
        }
        cache_size = n + 1;
    }
    return cache[n];
#endif
}
