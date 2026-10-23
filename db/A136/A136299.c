/*
 * A136299  a(n) = 3*a(n-1) - 4*a(n-3), with a(0)=1, a(1)=2, a(2)=4, a(3)=7.
 * https://oeis.org/A136299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) Initial values: a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=7
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 30, i64 n <= 61, i128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values 0 .. len-1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t len = 0;           /* number of computed terms */

    /* Quick return for the first three indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Initialise the cache on the first call */
    if (!cache) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        len = 4;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] - 4 * cache[i-3];
        }
        len = n + 1;
    }

    return cache[n];
}
