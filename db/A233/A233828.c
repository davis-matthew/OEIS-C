/*
 * A233828  a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3). a(0) = -1, a(1) = 1, a(2) =
 *          1.
 * https://oeis.org/A233828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3) Initial values: a(0) = -1,
 *            a(1) = 1, a(2) = 1
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 22, i64 n <= 43, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array */
static idx_t cache_cap = 0;      /* number of stored terms (cache_cap ==1 means a(0) stored, etc.) */

seq_t a(idx_t n)
{
    /* Handle the three explicit initial terms */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* Allocate the cache if needed and initialize the first three terms */
    if (!cache) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* OOM */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
    }

    /* Extend the cache up to the required index */
    while (cache_cap <= n) {
        seq_t val = 2 * cache[cache_cap - 1] +
                    2 * cache[cache_cap - 2] +
                    cache[cache_cap - 3];
        seq_t *tmp = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();  /* OOM */
        cache = tmp;
        cache[cache_cap] = val;
        ++cache_cap;
    }

    return cache[n];
}
