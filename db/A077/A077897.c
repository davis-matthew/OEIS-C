/*
 * A077897  Expansion of (1-x)^(-1)/(1+x-2*x^2-x^3).
 * https://oeis.org/A077897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=3, a(n)=1-a(n-1)+2*a(n-2)+a(n-3) (n>=3)
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077897: Expansion of (1-x)^(-1)/(1+x-2*x^2-x^3).  */

#include <stdlib.h>

/* The following function is called with the sequence index n (0‑based) and
 * returns the value a(n) in type seq_t.  Values are memoised so that
 * repeated calls for the same or larger indices are O(1) after the first
 * evaluation.  The implementation uses an iterative recurrence and
 * grows its internal storage as needed. */
seq_t a(idx_t n) {
    /* static data for memoisation */
    static seq_t *cache = NULL;          /* array of computed values */
    static size_t capacity = 0;          /* allocated array size */
    static idx_t last = (idx_t)-1;        /* highest index computed */

    /* Initialise on first call */
    if (cache == NULL) {
        /* allocate space for at least 3 values (indices 0..2) */
        size_t init = 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        capacity = init;
        /* set the first three terms explicitly */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        last = 2;
    }

    /* grow the array if the requested index is beyond current capacity */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute values up to the requested index if needed */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)0;
            } else if (i == 2) {
                cache[2] = (seq_t)3;
            } else {
                /* recurrence: a(i) = 1 - a(i-1) + 2*a(i-2) + a(i-3) */
                cache[i] = (seq_t)1 - cache[i-1]
                           + (seq_t)2 * cache[i-2]
                           + cache[i-3];
            }
        }
        last = n;
    }

    return cache[n];
}
