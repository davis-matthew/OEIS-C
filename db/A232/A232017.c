/*
 * A232017  Number of n X 2 0..2 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 13*a(n-2) + 16*a(n-3) + 7*a(n-4) + a(n-5) with
 *            initial values a(1)=3, a(2)=22, a(3)=121, a(4)=704, a(5)=4059
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <vector>
#endif

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Handle the empty/invalid region quietly */
    if (n < 1) return 0;

    /* Constant initial values for indices 1..5 */
    enum { INIT = 5 };
    static const seq_t init[INIT + 1] = {0, 3, 22, 121, 704, 4059};

    /* Cache for computed values.  The cache is resized lazily. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
#endif

    /* Ensure the cache has room for at least n+1 entries (index 0 unused). */
    if (n <= INIT) {
#ifdef __cplusplus
        if (cache.size() < (size_t)(INIT + 1))
            cache.resize(INIT + 1);   /* default-initialise to 0 */
#else
        if (cache_size < (size_t)(INIT + 1)) {
            cache = realloc(cache, (INIT + 1) * sizeof(seq_t));
            for (size_t i = cache_size; i < (size_t)(INIT + 1); ++i)
                cache[i] = 0;
            cache_size = INIT + 1;
        }
#endif
        return init[(size_t)n];
    }

    /* Resize cache if necessary */
#ifdef __cplusplus
    if (cache.size() <= n) cache.resize(n + 1);
#else
    if (cache_size <= n) {
        cache = realloc(cache, (n + 1) * sizeof(seq_t));
        for (size_t i = cache_size; i <= n; ++i)
            cache[i] = 0;
        cache_size = n + 1;
    }
#endif

    /* Fill in any missing entries up to n */
    size_t start = cache[1] != 0 ? 6 : 6;  /* after the known 5 entries */
#ifdef __cplusplus
    /* Initialise the first five entries if not already done */
    if (cache.size() >= 6 && cache[1] == 0) {
        cache[0] = 0; cache[1] = 3; cache[2] = 22; cache[3] = 121;
        cache[4] = 704; cache[5] = 4059;
    }
    for (; start <= n; ++start) {
        cache[start] =
            (seq_t)3 * cache[start - 1] +
            (seq_t)13 * cache[start - 2] +
            (seq_t)16 * cache[start - 3] +
            (seq_t)7 * cache[start - 4] +
            cache[start - 5];
    }
    return cache[n];
#else
    /* Initialise the first five entries if not already done */
    if (cache[1] == 0) {
        cache[0] = 0; cache[1] = 3; cache[2] = 22; cache[3] = 121;
        cache[4] = 704; cache[5] = 4059;
    }
    for (; start <= n; ++start) {
        cache[start] =
            (seq_t)3 * cache[start - 1] +
            (seq_t)13 * cache[start - 2] +
            (seq_t)16 * cache[start - 3] +
            (seq_t)7 * cache[start - 4] +
            cache[start - 5];
    }
    return cache[n];
#endif
}
