/*
 * A042853  Denominators of continued fraction convergents to sqrt(957).
 * https://oeis.org/A042853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 29698*a(n-6) - a(n-12) for n > 11. Initial values:
 *            a(0)=1, a(1)=1, a(2)=15, a(3)=31, a(4)=449, a(5)=480,
 *            a(6)=29249, a(7)=29729, a(8)=445455, a(9)=920639,
 *            a(10)=13334401, a(11)=14255040
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
    /* The first 12 terms are needed for the recurrence. */
    static const seq_t init[12] = {
        1, 1, 15, 31, 449, 480,
        29249, 29729, 445455, 920639,
        13334401, 14255040
    };

    /* Return a small value directly. */
    if (n < 12) {
        return init[(idx_t)n];
    }

    /* Extend the cache if necessary. */
    if (cache == NULL) {
        /* Allocate just enough space for the first 12 terms. */
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
        cached_len = 12;
    }

    if (n + 1 > cached_len) {
        seq_t *newcache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!newcache) abort();
        cache = newcache;
        /* Compute the new terms using the recurrence. */
        for (idx_t i = cached_len; i <= n; ++i) {
            /* i >= 12 because cached_len >= 12 and the else branch
               will only be reached for i >=12. */
            cache[i] = ((seq_t)29698) * cache[i - 6] - cache[i - 12];
        }
        cached_len = n + 1;
    }

    return cache[n];
}
