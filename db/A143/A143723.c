/*
 * A143723  Aliquot sequence starting at 52.
 * https://oeis.org/A143723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = A001065(a(n)), a(0) = 52 (Aliquot sequence starting at
 *            52)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* sum of proper divisors: σ(n) – n */
static seq_t sum_proper_divisors(seq_t x)
{
    if (x <= (seq_t)1)
        return (seq_t)0;
    seq_t sum = (seq_t)1;
    seq_t i   = (seq_t)2;
    while (i * i <= x) {
        if (x % i == (seq_t)0) {
            sum += i;
            seq_t d = x / i;
            if (d != i)
                sum += d;
        }
        i++;
    }
    return sum;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation: use a static vector for cache */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(32);
        cache.push_back((seq_t)52);          /* a(0) */
    }
    /* extend cache to reach index n */
    while ((idx_t)cache.size() <= n) {
        seq_t prev = cache.back();
        cache.push_back(sum_proper_divisors(prev));
    }
    return cache[(size_t)n];
#else
    /* C implementation: use malloc/realloc for cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    if (n >= size) {
        /* Ensure capacity */
        if (n + 1 > cap) {
            size_t new_cap = cap ? cap * 2 : 4;
            if (new_cap < (size_t)(n + 1))
                new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure fallback */
            cache = tmp;
            cap   = new_cap;
        }
        /* Compute missing terms */
        for (size_t i = size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)52;
            } else {
                cache[i] = sum_proper_divisors(cache[i - 1]);
            }
        }
        size = (size_t)n + 1;
    }
    return cache[(size_t)n];
#endif
}
