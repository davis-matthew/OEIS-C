/*
 * A164429  Number of binary strings of length n with no substrings equal to
 *          0000, 0011, or 1011.
 * https://oeis.org/A164429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164429
   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=13
   a(n) = a(n-1) + a(n-3) + 4   for n>4   */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: use std::vector to hold the cache */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(5);
        cache.push_back((seq_t)1);   /* a(0) */
        cache.push_back((seq_t)2);   /* a(1) */
        cache.push_back((seq_t)4);   /* a(2) */
        cache.push_back((seq_t)8);   /* a(3) */
        cache.push_back((seq_t)13);  /* a(4) */
    }

    if (n >= cache.size())
    {
        cache.resize(n + 1);
        for (idx_t i = 5; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 3] + (seq_t)4;
    }
    return cache[(size_t)n];
#else
    /* C build: dynamic array with malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache == NULL)
    {
        cache_cap = 5;                     /* indices 0..4 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)13;
    }

    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* allocation failure – unlikely */

        cache = tmp;
        for (idx_t i = cache_cap; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 3] + (seq_t)4;
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
#endif
}
