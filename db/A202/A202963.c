/*
 * A202963  Number of arrays of 3 integers in -n..n with sum zero and adjacent
 *          elements differing in absolute value.
 * https://oeis.org/A202963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4), with a(1)=2, a(2)=6,
 *            a(3)=20, a(4)=36
 * Range:     u8 n <= 9, u16 n <= 148, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;

#ifdef __cplusplus
    /* C++ case – seq_t may be a non‑POD type. */
    static std::vector<seq_t> cache;
    static idx_t max_computed = 0;

    /* Ensure the vector can hold indices up to n. */
    if (cache.size() < (size_t)n) cache.resize(n);

    /* Initialise the first four terms if not yet done. */
    if (max_computed < 4) {
        if (n >= 1) cache[0] = 2;
        if (n >= 2) cache[1] = 6;
        if (n >= 3) cache[2] = 20;
        if (n >= 4) cache[3] = 36;
        max_computed = (n >= 4 ? 4 : (idx_t)n);
        if (n == max_computed) return cache[n - 1];
    }

    /* Return if already known. */
    if (n <= max_computed) return cache[n - 1];

    /* Compute missing terms. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[(size_t)i - 1] = 2 * cache[(size_t)i - 2]
                               - 2 * cache[(size_t)i - 4]
                               + cache[(size_t)i - 5];
    }
    max_computed = n;
    return cache[n - 1];
#else
    /* C case – seq_t is a POD type, realloc works. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* Grow the buffer if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure, unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if not yet done. */
    if (max_computed < 4) {
        idx_t upto = (n < 4 ? n : 4);
        for (idx_t i = max_computed + 1; i <= upto; ++i) {
            switch (i) {
                case 1: cache[0] = 2; break;
                case 2: cache[1] = 6; break;
                case 3: cache[2] = 20; break;
                case 4: cache[3] = 36; break;
            }
        }
        if (upto > max_computed) max_computed = upto;
        if (n == max_computed) return cache[n - 1];
    }

    /* Return if already known. */
    if (n <= max_computed) return cache[n - 1];

    /* Compute missing terms. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[(size_t)i - 1] = 2 * cache[(size_t)i - 2]
                               - 2 * cache[(size_t)i - 4]
                               + cache[(size_t)i - 5];
    }
    max_computed = n;
    return cache[n - 1];
#endif
}
