/*
 * A069378  Number of n X 3 binary arrays with a path of adjacent 1's from top
 *          row to bottom row.
 * https://oeis.org/A069378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) - a(n-3) + 4*a(n-4) - 2*a(n-5) with
 *            a(1)=7, a(2)=37, a(3)=197, a(4)=1041, a(5)=5503
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
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
    if (n == 0) /* undefined, but avoid crash */
        return (seq_t)0;

    /* Initial five terms, stored 1‑based. */
    const seq_t init[6] = {0, 7, 37, 197, 1041, 5503};

#ifdef __cplusplus
    /* C++: use std::vector for automatic construction/destruction. */
    static std::vector<seq_t> cache;
    static idx_t max_cached = 0;

    if (cache.empty()) {
        cache.resize(6);                 /* indices 0..5 */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = init[i];
        max_cached = 5;
    }

    if (n <= max_cached)
        return cache[(size_t)n];

    if (cache.size() <= n)
        cache.resize((size_t)n + 1);

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)7 * cache[(size_t)(i - 1)]
                          - (seq_t)9 * cache[(size_t)(i - 2)]
                          - cache[(size_t)(i - 3)]
                          + (seq_t)4 * cache[(size_t)(i - 4)]
                          - (seq_t)2 * cache[(size_t)(i - 5)];
    }
    max_cached = n;
    return cache[(size_t)n];

#else /* C path */
    /* Use raw malloc/realloc; seq_t is a POD integer type. */
    static seq_t *cache = NULL;            /* cached values, 1‑based */
    static idx_t cache_size = 0;           /* allocated slots (unused index 0) */
    static idx_t max_cached = 0;           /* highest computed index */

    if (max_cached == 0) {                  /* first call – allocate minimal array */
        cache = malloc((size_t)6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache_size = 6;                       /* indices 0..5 */
        cache[0] = 0;
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = init[i];
        max_cached = 5;
    }

    if (n <= max_cached)
        return cache[(size_t)n];

    if (cache_size <= n) {                   /* grow storage */
        seq_t *tmp = realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing terms */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)7 * cache[(size_t)(i - 1)]
                          - (seq_t)9 * cache[(size_t)(i - 2)]
                          - cache[(size_t)(i - 3)]
                          + (seq_t)4 * cache[(size_t)(i - 4)]
                          - (seq_t)2 * cache[(size_t)(i - 5)];
    }
    max_cached = n;
    return cache[(size_t)n];
#endif
}
