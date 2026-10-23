/*
 * A187246  Number of cycles with 2 alternating runs in all permutations of
 *          [n] (it is assumed that the smallest element of the cycle is in
 *          the first position).
 * https://oeis.org/A187246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*a(n-1) - (3*n-1)*a(n-2) + 2*(n-2)*a(n-3) for n≥4,
 *            with a(0)=a(1)=a(2)=0, a(3)=1. The recurrence is D‑finite and
 *            can be evaluated in O(n) time with memoisation
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (n < cache_len)               /* already computed */
        return cache[n];

    /* enlarge cache to hold up to n */
    idx_t old_len   = cache_len;
    idx_t new_len   = (n + 1 > 4 ? n + 1 : 4);   /* at least 4 for base values */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    if (!new_cache) return 0;        /* out of memory – should never happen */
    for (idx_t i = 0; i < old_len; i++) new_cache[i] = cache[i];
    for (idx_t i = old_len; i < new_len; i++) new_cache[i] = 0;
    free(cache);
    cache     = new_cache;
    cache_len = new_len;

    /* initialise known base values */
    if (cache_len > 0) cache[0] = 0;
    if (cache_len > 1) cache[1] = 0;
    if (cache_len > 2) cache[2] = 0;
    if (cache_len > 3) cache[3] = 1;

    /* compute missing terms up to n */
    idx_t start = old_len;
    if (start < 4) start = 4;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term = (seq_t)(i + 3) * cache[i - 1]
                   - (seq_t)(3 * i - 1) * cache[i - 2]
                   + (seq_t)(2 * (i - 2)) * cache[i - 3];
        cache[i] = term;
    }
    return cache[n];
}
