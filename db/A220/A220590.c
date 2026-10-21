/*
 * A220590  Number of ways to reciprocally link elements of an n X 2 array
 *          either to themselves or to exactly two king-move neighbors.
 * https://oeis.org/A220590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) + 2*a(n-3) - 2*a(n-4) * Initial
 *            values: a(1)=1, a(2)=8, a(3)=33, a(4)=155 * * The implementation
 *            caches previously computed terms so that * any order of calls is
 *            handled efficiently. The caching logic * is implemented
 *            separately for C and C++ builds to satisfy * the dif...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
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
    /* a(0) is defined as 0 for convenience, though the sequence
     * starts at offset 1. */
    if (n == 0) return (seq_t)0;

#if defined(__cplusplus)
    /* C++ build – use std::vector for automatic construction
     * and proper reallocation of potentially non‑trivial seq_t
     * types. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(5);           /* indices 0..4 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)155;
    }

    /* Grow the vector if needed */
    if (n >= cache.size()) {
        size_t oldSize = cache.size();       /* current capacity */
        cache.resize(n + 1);                 /* new capacity */
        for (idx_t i = oldSize; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1]
                       + (seq_t)7 * cache[i - 2]
                       + (seq_t)2 * cache[i - 3]
                       - (seq_t)2 * cache[i - 4];
        }
    }
    return cache[(size_t)n];
#else
    /* C build – use malloc/realloc; seq_t is a plain integer type. */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static idx_t cap   = 0;        /* largest n with a valid cache[n] */

    if (!cache) {
        /* Allocate room for indices 0..4 */
        cache = (seq_t *)calloc(5, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);   /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)155;
        cap = 4;
    }

    if (n > cap) {
        /* Grow the cache to size n+1 */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* Initialize newly allocated positions to 0 for safety */
        for (idx_t i = cap + 1; i <= n; ++i) cache[i] = (seq_t)0;
        /* Compute terms from cap+1 up to n */
        for (idx_t i = cap + 1; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1]
                       + (seq_t)7 * cache[i - 2]
                       + (seq_t)2 * cache[i - 3]
                       - (seq_t)2 * cache[i - 4];
        }
        cap = n;
    }
    return cache[n];
#endif
}
