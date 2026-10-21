/*
 * A078071  Expansion of (1-x)/(1+2*x+2*x^2+2*x^3).
 * https://oeis.org/A078071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-3, a(2)=4, a(n) = -2*(a(n-1)+a(n-2)+a(n-3)) for
 *            n>2
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expands (1 - x)/(1 + 2x + 2x² + 2x³). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stores computed terms */
    static idx_t capacity = 0;    /* allocated size of cache      */
    static idx_t size = 0;        /* number of cached terms       */

    /* Initialisation: set first three values and allocate memory */
    if (size == 0) {
        capacity = (n < 16 ? 16 : n + 1);          /* enough for current n */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)4;
        size = 3;
    }

    /* If the requested term is already cached, return it immediately */
    if (n < size) {
        return cache[n];
    }

    /* Grow the cache if needed */
    if (capacity <= n) {
        idx_t new_cap = capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute terms incrementally using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        seq_t s = cache[i - 1] + cache[i - 2] + cache[i - 3];
        cache[i] = (seq_t)(-2) * s;
    }
    size = n + 1;                 /* update cache size */

    return cache[n];
}
