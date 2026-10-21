/*
 * A202900  Number of n X 2 0..1 arrays with every one equal to some NW, E or
 *          S neighbor.
 * https://oeis.org/A202900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=14
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* initialize base terms on first call */
    if (!cache) {
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t*)calloc(4U, sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        max_cached = 3;
    }

    if (n <= max_cached)
        return cache[n];

    /* enlarge cache to hold up to n */
#ifdef __cplusplus
    {
        seq_t *tmp = new seq_t[(size_t)(n + 1)];
        for (idx_t i = 0; i <= max_cached; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
    }
#else
    {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
    }
#endif

    /* compute new terms using recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t diff = cache[i - 1] - cache[i - 2] + cache[i - 3] - cache[i - 4];
        cache[i] = diff * (seq_t)4;
    }

    max_cached = n;
    return cache[n];
}
