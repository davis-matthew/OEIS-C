/*
 * A180146  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: 1/(1 -
 *          4*x - 3*x^2 + 6*x^3).
 * https://oeis.org/A180146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) - 6*a(n-3) a(0)=1, a(1)=4, a(2)=19
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with the three seed values if it does not exist */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t*)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)19;
    }

    /* if the requested index is already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend the cache to accommodate indices up to n */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL) return 0;                 /* allocation failure */
    cache = tmp;

    /* compute all new terms using the linear recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 4*a(i-1) + 3*a(i-2) - 6*a(i-3) */
        cache[i] = cache[i-1] * 4 + cache[i-2] * 3 - cache[i-3] * 6;
    }

    cache_len = n + 1;
    return cache[n];
}
