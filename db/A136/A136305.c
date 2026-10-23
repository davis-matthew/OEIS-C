/*
 * A136305  Expansion of g.f. (3 -x +2*x^2)/(1 -3*x +2*x^2 -x^3).
 * https://oeis.org/A136305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n) * Initial values: a(0) = 3,
 *            a(1) = 8, a(2) = 20
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static idx_t   capacity = 0;         /* allocated size */
    static idx_t   filled   = 0;         /* number of valid elements */

    /* initialise with first three terms */
    if (filled == 0) {
        capacity = 3;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;   /* failed allocation – not expected in tests */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)20;
        filled = 3;
    }

    /* ensure the array is large enough for n */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 4;
        if (new_cap < n + 1) new_cap = n + 1;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – not expected */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to n */
    if (n + 1 > filled) {
        for (idx_t i = filled; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] - 2 * cache[i - 2] + cache[i - 3];
        }
        filled = n + 1;
    }

    return cache[n];
}
