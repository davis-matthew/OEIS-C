/*
 * A057597  a(n) = -a(n-1) - a(n-2) + a(n-3), a(0)=0, a(1)=0, a(2)=1.
 * https://oeis.org/A057597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - a(n-2) + a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     i8 n <= 18, i16 n <= 37, i32 n <= 73, i64 n <= 145,
 *            i128 n <= 291, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* lazily initialise cache with first three terms */
    if (!cache) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
    }

    /* easy shortcut for early indices */
    if ((size_t)n < cap)
        return cache[(size_t)n];

    /* extend cache up to n */
    size_t need = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    for (size_t i = cap; i <= (size_t)n; ++i) {
        cache[i] = -cache[i-1] - cache[i-2] + cache[i-3];
    }
    cap = need;
    return cache[(size_t)n];
}
