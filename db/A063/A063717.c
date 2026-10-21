/*
 * A063717  a(n) is the greatest divisor of n^2 that is less than n.
 * https://oeis.org/A063717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d | d divides n^2 and d < n } (OEIS A063717)
 * Range:     u8 n <= 15, u16 n <= 287, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoisation array: cache[k] holds a(k) for k < size. */
    static seq_t *cache = NULL;
    static idx_t size = 0;       /* first unused index */
    static idx_t cap = 0;        /* allocated capacity */

    /* No useful values below the offset (n<2). Return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Initialise storage on first call. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return (seq_t)0;
        /* Optional: fill with zeros for indices 0 and 1. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        size = 2;
    }

    /* Expand the cache if necessary. */
    if (n + 1 > cap) {
        idx_t newcap = cap;
        while (newcap < n + 1)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* Compute any missing values up to n. */
    for (idx_t k = size; k <= n; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;        /* k² */
        idx_t d = k - 1;                       /* start just below k */
        /* Search downward until a divisor is found. */
        for (;;) {
            if (sq % (seq_t)d == 0) {
                cache[k] = (seq_t)d;
                break;
            }
            if (d == 1) {                     /* shouldn't happen for k≥2 */
                cache[k] = (seq_t)1;
                break;
            }
            --d;
        }
    }
    size = n + 1;
    return cache[n];
}
