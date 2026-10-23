/*
 * A117575  Expansion of (1-x^3)/((1-x)*(1+2*x^2)).
 * https://oeis.org/A117575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3); Initial values: a(0)=1,
 *            a(1)=1, a(2)=-1, a(3)=-2
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated length */
    static idx_t cache_len = 0;   /* actual filled length (next free index) */

    /* allocate/expand cache capacity if needed */
    if (cache == NULL) {
        cache_cap = (n < 4 ? 4 : n + 1);      /* at least 4 slots for bases */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        while (cache_cap <= n) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }

    /* initialise base values if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)-1;
        } else if (i == 3) {
            cache[3] = (seq_t)-2;
        } else {
            cache[i] = cache[i-1] - cache[i-2] * (seq_t)2 + cache[i-3] * (seq_t)2;
        }
    }
    cache_len = n + 1;          /* all entries up to n are now populated */

    return cache[n];
}
