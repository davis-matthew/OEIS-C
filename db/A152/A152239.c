/*
 * A152239  a(n) = -5*a(n-1) + 7*a(n-2) for n > 1 with a(0) = 1 and a(1) = -7.
 * https://oeis.org/A152239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -5*a(n-1) + 7*a(n-2); a(0)=1, a(1)=-7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 24, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases handled immediately */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-7;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t last = 1;   /* Highest index computed so far */

    /* Initialise cache on first use */
    if (capacity == 0) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-7;
    }

    /* Grow cache if necessary */
    if (n + 1 > capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = (idx_t)new_cap;
    }

    /* Compute any missing terms up to index n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = ((seq_t)-5) * cache[i - 1] + ((seq_t)7) * cache[i - 2];
    }

    last = n;
    return cache[n];
}
