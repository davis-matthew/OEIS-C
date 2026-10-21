/*
 * A061547  Number of 132 and 213-avoiding derangements of {1,2,...,n}.
 * https://oeis.org/A061547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2)+2 (n≥3), a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation to compute each term only once. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;
    static idx_t capacity = 0;  /* total allocated slots */
    static idx_t stored   = 0;  /* number of terms stored (indices 0 .. stored-1) */

    /* initialisation */
    if (cache == NULL) {
        capacity = 16;                    /* initial arbitrary size */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL) {
            /* out of memory – terminate the program, as later use would be unsound */
            abort();
        }
        /* base terms */
        cache[0] =  (seq_t)1;
        cache[1] =  (seq_t)0;
        cache[2] =  (seq_t)1;
        stored   = 3;
    }

    /* ensure cache large enough for the requested index */
    if (n >= capacity) {
        while (capacity <= n) capacity <<= 1;                 /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = stored; i <= n; ++i) {
        if (i >= 3) {
            /* recurrence: a(i) = 4*a(i-2) + 2 */
            cache[i] = 4 * cache[i-2] + (seq_t)2;
        }
    }
    if (n >= stored) stored = n + 1;

    return cache[n];
}
