/*
 * A078041  Expansion of (1-x)/(1+x-x^2-2*x^3).
 * https://oeis.org/A078041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 40, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) =  1,  a(1) = -2,  a(2) =  3  
    a(n) = -a(n-1) + a(n-2) + 2*a(n-3)   for n ≥ 3
    (derived from (1‑x)/(1+x‑x²‑2x³)) */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache    = NULL;   /* allocated array of terms   */
    static size_t size      = 0;     /* number of terms actually computed */
    static size_t capacity  = 0;     /* current allocation size       */

    /* grow cache if necessary (ensure capacity ≥ n+1) */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;      /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms as needed */
    if (size <= (size_t)n) {
        for (size_t k = size; k <= (size_t)n; ++k) {
            seq_t val;
            if (k == 0) val = (seq_t)1;
            else if (k == 1) val = (seq_t)-2;
            else if (k == 2) val = (seq_t)3;
            else
                val = -cache[k-1] + cache[k-2] + (seq_t)2 * cache[k-3];
            cache[k] = val;
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
