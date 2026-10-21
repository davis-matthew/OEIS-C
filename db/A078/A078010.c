/*
 * A078010  Expansion of (1-x)/(1 - x - x^2 - 2*x^3).
 * https://oeis.org/A078010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) for n > 2 with initial values
 *            a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = (idx_t)-1;

    /* initialize cache with first three terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cap  = 3;
        last = 2;
    }

    if (n < 3)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap << 1;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (seq_t)2 * cache[i-3];
    }
    last = n;

    return cache[n];
}
