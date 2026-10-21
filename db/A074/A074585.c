/*
 * A074585  a(n)= Sum_{j=0..floor(n/2)} A073145(2*j + q), where q = 2*(n/2 -
 *          floor(n/2)).
 * https://oeis.org/A074585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + 2*a(n-3) + a(n-4) - a(n-5), a(0)=3, a(1)=-1,
 *            a(2)=2, a(3)=4, a(4)=-3
 * Range:     i8 n <= 15, i16 n <= 32, i32 n <= 71, i64 n <= 142,
 *            i128 n <= 289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t length   = 0;   /* number of computed terms (indices 0..length-1) */
    static idx_t capacity = 0;   /* allocated array size */

    /* allocate and/or grow cache */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* initialise first five terms if not yet done */
    if (length < 5) {
        cache[0] = 3;
        cache[1] = -1;
        cache[2] = 2;
        cache[3] = 4;
        cache[4] = -3;
        length   = 5;
    }

    /* compute missing terms */
    for (idx_t i = length; i <= n; ++i) {
        cache[i] = -cache[i-1] + 2*cache[i-3] + cache[i-4] - cache[i-5];
    }

    length = n + 1;
    return cache[n];
}
