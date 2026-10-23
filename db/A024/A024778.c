/*
 * A024778  Expansion of 1/((1-x)(1-8x)(1-9x)(1-12x)).
 * https://oeis.org/A024778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 305*a(n-2) + 1140*a(n-3) - 864*a(n-4) initial
 *            values: a(0)=1, a(1)=30, a(2)=595, a(3)=9840
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic
       construction/destruction of arbitrary‑precision types. */
    static std::vector<seq_t> cache;
    static bool initialized = false;
    if (!initialized) {
        cache.resize(4);
        cache[0] = seq_t(1);
        cache[1] = seq_t(30);
        cache[2] = seq_t(595);
        cache[3] = seq_t(9840);
        initialized = true;
    }
    if ((size_t)n < cache.size())
        return cache[n];
    for (idx_t i = (idx_t)cache.size(); i <= n; ++i) {
        seq_t value = seq_t(30) * cache[i - 1]
                    - seq_t(305) * cache[i - 2]
                    + seq_t(1140) * cache[i - 3]
                    - seq_t(864) * cache[i - 4];
        cache.push_back(value);
    }
    return cache[n];
#else
    /* C implementation using a dynamically resized array. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure initial values are in place. */
    if (cache_len < 4) {
        if (cache_cap < 4) {
            cache_cap = 4;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)595;
        cache[3] = (seq_t)9840;
        cache_len = 4;
    }

    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow array if necessary. */
    if (cache_cap < (idx_t)(n + 1)) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (idx_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)30 * cache[i - 1]
                 - (seq_t)305 * cache[i - 2]
                 + (seq_t)1140 * cache[i - 3]
                 - (seq_t)864 * cache[i - 4];
    }
    cache_len = n + 1;
    return cache[n];
#endif
}
