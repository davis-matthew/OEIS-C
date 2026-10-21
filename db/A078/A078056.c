/*
 * A078056  Expansion of (1-x)/(1+2*x-x^2-x^3).
 * https://oeis.org/A078056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) + a(n-3) Initial values: a(0)=1,
 *            a(1)=-3, a(2)=7
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t size   = 0;

    /* first call: initialise three seed values */
    if (!cache) {
        size  = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        cache[0] = 1;
        cache[1] = -3;
        cache[2] = 7;
    }

    /* if the value is already known, return it */
    if (n < size)
        return cache[n];

    /* enlarge cache to hold up to n */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);

    /* compute missing terms using the recurrence */
    for (idx_t i = size; i <= n; ++i)
        cache[i] = -2 * cache[i-1] + cache[i-2] + cache[i-3];

    size = new_size;
    return cache[n];
}
