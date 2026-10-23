/*
 * A018055  Expansion of 1/((1-3*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A018055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 141*a(n-2) + 252*a(n-3); a(0) = 1, a(1) = 22,
 *            a(2) = 343
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* if already computed, just return */
    if (n < cache_len)
        return cache[n];

    /* need to extend cache to hold index n */
    idx_t new_len = n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    }

    /* initialize base values if they are not already set */
    if (new_len > 0)          cache[0] = (seq_t)1;
    if (new_len > 1)          cache[1] = (seq_t)22;
    if (new_len > 2)          cache[2] = (seq_t)343;

    /* compute new entries only if necessary */
    idx_t start = cache_len;
    if (start <= 3)           start = 3;          /* indices 0..2 are base values */

    for (idx_t i = start; i < new_len; ++i)
    {
        /* recurrence: a(i) = 22*a(i-1) - 141*a(i-2) + 252*a(i-3) */
        cache[i] = (seq_t)22 * cache[i - 1];
        cache[i] -= (seq_t)141 * cache[i - 2];
        cache[i] += (seq_t)252 * cache[i - 3];
    }

    cache_len = new_len;
    return cache[n];
}
