/*
 * A192803  Coefficient of x^2 in the reduction of the polynomial (x+2)^n by
 *          x^3->x^2+x+1.
 * https://oeis.org/A192803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 11*a(n-3) Initial values: a(0)=0,
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();              /* memory allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }

    /* if the requested n is already cached, return it immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache to hold up to n (inclusive) */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (tmp == NULL) abort();                  /* memory allocation failed */
    cache = tmp;

    /* initialise newly allocated slots to zero */
    for (idx_t i = old_size; i < cache_size; ++i) {
        cache[i] = (seq_t)0;
    }

    /* compute terms from old_size up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        /* compute a(i) = 7*a(i-1) - 15*a(i-2) + 11*a(i-3) */
        seq_t t1 = cache[i-1] * (seq_t)7;
        seq_t t2 = cache[i-2] * (seq_t)15;
        seq_t t3 = cache[i-3] * (seq_t)11;

        /* safely perform addition and subtraction */
        seq_t res = t1 + t3;
        res -= t2;  /* the recurrence guarantees this stays non‑negative */
        cache[i] = res;
    }

    return cache[n];
}
