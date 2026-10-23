/*
 * A020566  Expansion of 1/((1-5x)(1-9x)(1-12x)).
 * https://oeis.org/A020566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) -213*a(n-2) +540*a(n-3), a(0)=1, a(1)=26,
 *            a(2)=463
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of elements stored in cache */

    /* Fast return if we already have the term */
    if (n < cached_len) return cache[n];

    /* Handle the first three terms directly */
    if (n < 3) {
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)26;
        return (seq_t)463;   /* n == 2 */
    }

    /* Allocate cache if it's not yet created */
    if (cache == NULL) {
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * newsize);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)463;
        cached_len = 3;
    } else if (cached_len <= n) {
        /* Grow cache to accommodate up to n */
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
        if (!cache) return (seq_t)0;
    }

    /* Compute the remaining terms */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t term = (seq_t)26 * cache[i-1] + (seq_t)540 * cache[i-3];
        term -= (seq_t)213 * cache[i-2];
        cache[i] = term;
    }
    cached_len = n + 1;

    return cache[n];
}
