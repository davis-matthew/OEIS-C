/*
 * A206948  Number of nonisomorphic graded posets with 0 and non-uniform Hasse
 *          graph of rank n, with exactly 2 elements of each rank level above
 *          0.
 * https://oeis.org/A206948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 40*a(n-2) + 55*a(n-3) - 30*a(n-4) + 6*a(n-5)
 *            with a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=19, a(5)=131
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoised cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* ensure the cache is large enough */
    if (n + 1 > cache_len) {
        size_t new_len = (cache_len == 0) ? 6 : cache_len;
        while (new_len < n + 1) new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();        /* out‑of‑memory */
        cache = tmp;
        cache_len = new_len;
        /* initialize newly allocated slots to zero */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = (seq_t)0;
    }

    /* initialise base cases (may be overwritten but harmless) */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)19;
    cache[5] = (seq_t)131;

    /* compute up to n if necessary */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)11 * cache[i-1];
        term -= (seq_t)40 * cache[i-2];
        term += (seq_t)55 * cache[i-3];
        term -= (seq_t)30 * cache[i-4];
        term += (seq_t)6  * cache[i-5];
        cache[i] = term;
    }

    return cache[(size_t)n];
}
