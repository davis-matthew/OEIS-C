/*
 * A186459  Number of (n+1) X 7 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) for n>12 Initial
 *            values a(1)..a(12) are given explicitly
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 41, u64 n <= 95, u128 n <= 200,
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
    /* Initial 12 terms of the sequence.  */
    static const seq_t init[12] = {
        151ULL, 197ULL, 362ULL, 673ULL, 1061ULL, 1693ULL,
        2377ULL, 3677ULL, 5073ULL, 7977ULL, 11533ULL, 18221ULL
    };

    /* For n <= 12 just return the hard‑coded value.  */
    if (n <= 12)
        return init[n - 1];

#ifdef __cplusplus
    /* C++ path – use std::vector to store the cache.  */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(1);            // unused element at index 0
        cache.insert(cache.end(), init, init + 12);
    }
    if (cache.size() <= n) {
        cache.resize(n + 1);
        for (size_t i = 13; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1];
            t1 += t1;                     // 2*a(i-1)
            seq_t t2 = cache[i - 3];
            t2 += t2;                     // 2*a(i-3)
            t1 -= t2;
            t2 = cache[i - 4];
            t2 += t2;                     // 2*a(i-4)
            t1 += t2;
            t1 -= cache[i - 5];           // -a(i-5)
            cache[i] = t1;
        }
    }
    return cache[(size_t)n];
#else
    /* C path – use a dynamically allocated array.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size (index 0..cache_cap-1) */
    if (!cache) {
        cache = calloc(13, sizeof(seq_t));   /* space for indices 0..12  */
        if (!cache) return 0;                /* out of memory */
        for (size_t i = 1; i <= 12; ++i)
            cache[i] = init[i - 1];
        cache_cap = 13;
    }
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* out of memory */
        cache = tmp;
        /* Zero new entries to allow safe arithmetic.  */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }
    /* Compute missing terms up to n.  */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 1];
        t1 += t1;                 /* 2*a(i-1) */
        seq_t t2 = cache[i - 3];
        t2 += t2;                 /* 2*a(i-3) */
        t1 -= t2;
        t2 = cache[i - 4];
        t2 += t2;                 /* 2*a(i-4) */
        t1 += t2;
        t1 -= cache[i - 5];       /* -a(i-5) */
        cache[i] = t1;
    }
    return cache[(size_t)n];
#endif
}
