/*
 * A079319  a(0) = 1; for n >= 1, a(n) = 4*a(n-1) - (2^n - 1).
 * https://oeis.org/A079319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + (4^n - 1)/3 Recurrence: a(0) = 1; a(n) = 4*a(n-1) -
 *            (2^n - 1) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;     /* cache[0] = 1 */
    static seq_t pow_two = (seq_t)1;/* 2^current_max_n, initially 2^0 */
    static idx_t  max_n   = 0;       /* largest n for which cache is filled */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache   = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;         /* a(0) = 1 */
    }

    /* if the term is already cached, return it immediately */
    if (n <= max_n) {
        return cache[n];
    }

    /* extend storage to hold up to a(n) */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

    /* compute missing terms from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* update power of two: 2^i */
        pow_two <<= 1;                     /* pow_two = 2^i */

        /* a(i) = 4*a(i-1) - (2^i - 1) */
        seq_t term = cache[i - 1] << 2;    /* 4*a(i-1) */
        term -= (pow_two - (seq_t)1);     /* subtract (2^i - 1) */
        cache[i] = term;
    }

    max_n = n;
    return cache[n];
}
