/*
 * A017894  Expansion of 1/(1-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17-x^18).
 * https://oeis.org/A017894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)+a(n-16)+a(n-17)+a(n-18)
 *            for n > 17; a(0) = 1, a(1..9) = 0, a(10..18) = 1. This
 *            implements the recurrence with memoization
 * Range:     u8 n <= 49, u16 n <= 83, u32 n <= 151, u64 n <= 287,
 *            u128 n <= 559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements currently stored */

    /* first call – initialise cache for indices 0..18 */
    if (cache_size == 0) {
        cache_size = 19;   /* covers 0 through 18 inclusive */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();   /* simple error handling */

        /* a(0) = 1, a(1..9) = 0, a(10..18) = 1 */
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < cache_size; ++i) {
            if (i >= 10) cache[i] = (seq_t)1;
            else         cache[i] = (seq_t)0;
        }
    }

    /* value already computed */
    if (n < cache_size) return cache[n];

    /* need to expand cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();   /* simple error handling */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* add a(i-10) ... a(i-18) if defined */
        if (i >= 10) sum += cache[i - 10];
        if (i >= 11) sum += cache[i - 11];
        if (i >= 12) sum += cache[i - 12];
        if (i >= 13) sum += cache[i - 13];
        if (i >= 14) sum += cache[i - 14];
        if (i >= 15) sum += cache[i - 15];
        if (i >= 16) sum += cache[i - 16];
        if (i >= 17) sum += cache[i - 17];
        if (i >= 18) sum += cache[i - 18];
        cache[i] = sum;
    }

    cache_size = new_size;   /* new computed range is fully valid */

    return cache[n];
}
