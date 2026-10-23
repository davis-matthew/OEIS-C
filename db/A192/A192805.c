/*
 * A192805  Constant term in the reduction of the polynomial 1+x+x^2+...+x^n
 *          by x^3->x^2+2x+1. See Comments.
 * https://oeis.org/A192805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) – a(n-3) – a(n-4), a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#  include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for arbitrary‑precision types */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(4);
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)1); /* a(1) */
        cache.push_back((seq_t)1); /* a(2) */
        cache.push_back((seq_t)2); /* a(3) */
    }
    if (n >= cache.size()) {
        cache.resize(n + 1);
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3] - cache[i-4];
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of valid elements */

    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache_size = 4;
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)1; /* a(1) */
        cache[2] = (seq_t)1; /* a(2) */
        cache[3] = (seq_t)2; /* a(3) */
    }

    if (n >= cache_size) {
        size_t newcap = cache_cap;
        while (newcap <= (size_t)n) newcap <<= 1;      /* grow exponentially */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                      /* allocation failure */
        cache = tmp;
        cache_cap = newcap;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3] - cache[i-4];
        }
        cache_size = n + 1;
    }
    return cache[(size_t)n];
#endif
}
