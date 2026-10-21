/*
 * A203357  Number of n X 3 binary arrays with every 1 immediately preceded by
 *          0 to the left or above.
 * https://oeis.org/A203357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 12*a(n-2) + 6*a(n-3) + a(n-4); a(1) = 3, a(2)
 *            = 20, a(3) = 102, a(4) = 565 (OEIS A203357, offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n < 1, but harmless */

#ifdef __cplusplus
    /* C++ implementation: use std::vector for simplicity */
    static std::vector<seq_t> cache(5, 0);  /* indices 0..4 */
    static idx_t max_comp = 0;            /* largest n already computed */

    if (cache.size() == 5 && max_comp == 0) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)102;
        cache[4] = (seq_t)565;
        max_comp = 4;
    }

    /* Grow and fill missing terms */
    while (max_comp < n) {
        seq_t val = 3 * cache[max_comp] + 12 * cache[max_comp-1]
                  + 6 * cache[max_comp-2] + cache[max_comp-3];
        cache.push_back(val);
        ++max_comp;
    }

    return cache[(size_t)n];
#else
    /* C implementation: dynamic array using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* capacity of the array (one past last index) */
    static idx_t  max_comp = 0; /* largest n already computed */

    if (cache == NULL) {
        /* initial capacity sufficient for indices 0..4 */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();        /* allocation failure */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)102;
        cache[4] = (seq_t)565;
        max_comp = 4;
    }

    /* Resize if necessary */
    while ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] + 12 * cache[i-2]
                 + 6 * cache[i-3] + cache[i-4];
    }
    if (n > max_comp) max_comp = n;

    return cache[(size_t)n];
#endif
}
