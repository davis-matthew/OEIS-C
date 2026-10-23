/*
 * A100734  a(n) = (5*n)!.
 * https://oeis.org/A100734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ path: use a std::vector to handle arbitrary‑precision types */
    static std::vector<seq_t> cache;
    static idx_t max_cached = 0;

    if (cache.empty()) {
        cache.reserve(1);
        cache.push_back((seq_t)1);      /* a(0) = (5*0)! = 0! = 1 */
    }

    if (n >= cache.size()) {
        cache.resize(n + 1);
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            seq_t val = cache[k - 1];
            idx_t start = 5 * (k - 1) + 1;
            idx_t finish = 5 * k;
            for (idx_t t = start; t <= finish; ++t) {
                val *= (seq_t)t;
            }
            cache[k] = val;
        }
        max_cached = n;
    }

    return cache[n];
#else
    /* C path: use malloc/realloc for plain integer types */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_cached = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_size = 1;
        max_cached = 0;
    }

    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_size = n + 1;
    }

    for (idx_t k = max_cached + 1; k <= n; ++k) {
        seq_t val = cache[k - 1];
        idx_t start = 5 * (k - 1) + 1;
        idx_t finish = 5 * k;
        for (idx_t t = start; t <= finish; ++t) {
            val *= (seq_t)t;
        }
        cache[k] = val;
    }
    max_cached = n;
    return cache[n];
#endif
}
