/*
 * A120514  a(n) = min{j : A120503(j) = n}.
 * https://oeis.org/A120514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n-1 + a( floor((n-1)/3)+1 ), a(1) = 1
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS sequence A120514.
 * Cache results so that subsequent calls are fast and
 * work for any order of n. */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;     /* array indexed from 1 */
    static size_t capacity = 0;     /* allocated size of cache */
    static size_t last = 0;         /* last index computed */

    if (n == 0)                      /* n should be >= 1, but guard */
        return 0;

    /* Make sure the cache is large enough */
    if (n + 1 > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[i] = 1;                     /* a(1) = 1 */
        } else {
            size_t j = (i - 1) / 3 + 1;        /* floor((i-1)/3)+1 */
            cache[i] = (seq_t)(i - 1) + cache[j];   /* a(i) = (i-1) + a(j) */
        }
    }
    last = (size_t)n;
    return cache[n];
}
