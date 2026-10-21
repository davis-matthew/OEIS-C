/*
 * A077925  Expansion of 1/((1-x)*(1+2*x)).
 * https://oeis.org/A077925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-2)^(n+1)) / 3 Recurrence: a(0) = 1 a(n) = -2 *
 *            a(n-1) + 1 (for n >= 1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    // Use std::vector for memoization in C++ builds
    static std::vector<seq_t> cache{ (seq_t)1 };   // a(0) = 1
    while (cache.size() <= n)
    {
        seq_t next = -2 * cache.back() + (seq_t)1;
        cache.push_back(next);
    }
    return cache[(size_t)n];
#else
    // Use malloc/realloc for memoization in C builds
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;      // a(0) = 1
        cache_len = 1;
    }
    while (cache_len <= n)
    {
        seq_t next = -2 * cache[cache_len - 1] + (seq_t)1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        cache[cache_len] = next;
        ++cache_len;
    }
    return cache[(size_t)n];
#endif
}
