/*
 * A063270  a(n) = 9^(2*n) + 1.
 * https://oeis.org/A063270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^(2*n) + 1 Recurrence: a(0)=2, a(1)=82, a(n) = 82*a(n-1)
 *            - 81*a(n-2) (n ≥ 2) The required value is returned as type
 *            seq_t; the implementation memoises terms to support unordered
 *            calls efficiently
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
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
    /* C++ mode: use a static std::vector for memoisation. */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(4);
        cache.push_back((seq_t)2);   /* a(0) */
        cache.push_back((seq_t)82);  /* a(1) */
    }
    while (cache.size() <= n)
    {
        seq_t next = (seq_t)82 * cache[cache.size() - 1]
                   - (seq_t)81 * cache[cache.size() - 2];
        cache.push_back(next);
    }
    return cache[n];
#else
    /* C mode: dynamic array with malloc/realloc. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (cache_size == 0)
    {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)2;   /* a(0) */
        cache[1] = (seq_t)82;  /* a(1) */
    }
    while (cache_size <= n)
    {
        idx_t sz = cache_size;
        cache = (seq_t *)realloc(cache, (sz + 1) * sizeof(seq_t));
        cache[sz] = (seq_t)82 * cache[sz - 1]
                  - (seq_t)81 * cache[sz - 2];
        cache_size = sz + 1;
    }
    return cache[n];
#endif
}
