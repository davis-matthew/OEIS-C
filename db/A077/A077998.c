/*
 * A077998  Expansion of (1-x)/(1-2*x-x^2+x^3).
 * https://oeis.org/A077998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) (n >= 3) initial values:
 *            a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest n for which cache[n] is valid */

    /* handle the three base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* initialise cache with the first three terms if needed */
    if (!cache)
    {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        max_computed = 2;
    }

    /* return cached value if already computed */
    if (n <= max_computed)
        return cache[n];

    /* extend the cache up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return 0;                       /* allocation failure – unlikely in tests */
    cache = tmp;

    /* fill in missing entries using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) + a(i-2) - a(i-3) */
        cache[i] = ( (seq_t)2 * cache[i-1] ) + cache[i-2] - cache[i-3];
    }

    max_computed = n;
    return cache[n];
}
