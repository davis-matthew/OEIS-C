/*
 * A233085  Number of 4Xn 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally or antidiagonally, top left element zero,
 *          and 1 appearing before 2 in row major order.
 * https://oeis.org/A233085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*a(n-1) - 450*a(n-2) + 1466*a(n-3) - 1853*a(n-4) +
 *            789*a(n-5) for n > 8 Initial values: a(1)=36, a(2)=1094,
 *            a(3)=41675, a(4)=1588262, a(5)=60632429, a(6)=2315298290,
 *            a(7)=88417171553, a(8)=3376534087742
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* stored initial values (index 0 unused) */
static const seq_t _a_init[9] = {
    0ull,
    36ull,
    1094ull,
    41675ull,
    1588262ull,
    60632429ull,
    2315298290ull,
    88417171553ull,
    3376534087742ull
};

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n <= 8) return _a_init[n];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;  /* largest index computed so far */
    static size_t cache_cap  = 0;  /* number of slots allocated */

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap  = 9;            /* capacity for indices 0..8 */
        cache_size = 8;            /* values up to index 8 are ready */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        for (size_t i = 0; i < cache_cap; ++i) {
            cache[i] = _a_init[i];
        }
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t t1 = 49 * cache[k-1];
        seq_t t2 = 450 * cache[k-2];
        seq_t t3 = 1466 * cache[k-3];
        seq_t t4 = 1853 * cache[k-4];
        seq_t t5 = 789 * cache[k-5];
        cache[k] = t1 + t3 + t5 - t2 - t4;
    }
    cache_size = n;

    return cache[n];
}
