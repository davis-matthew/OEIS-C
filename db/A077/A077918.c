/*
 * A077918  Expansion of (1 - x)^(-1)/(1 + 2*x - 2*x^2 + x^3).
 * https://oeis.org/A077918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 42, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A077918
 *
 * Generating function: (1-x)^(-1) / (1 + 2x - 2x^2 + x^3)
 *
 * Recurrence (order 4):
 *   a(0)=1, a(1)=-1, a(2)=5, a(3)=-12
 *   a(n) = -a(n-1) + 4*a(n-2) - 3*a(n-3) + a(n-4)   (n ≥ 4)
 * This recurrence follows from extracting the coefficients of the power
 * series of the given rational function.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t   list_size = 0;  /* number of terms stored (highest index +1) */
    static idx_t   capacity  = 0;  /* allocated size of cache */

    /* Ensure sufficient capacity for requested term */
    if (n + 1 > capacity) {
        idx_t new_cap = (capacity == 0) ? 8 : capacity * 2;
        while (new_cap <= (idx_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise first 4 terms once */
    if (list_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)-12;
        list_size = 4;        /* we now have a[0] … a[3] */
    }

    /* If the requested term is already computed, return it */
    if (n < list_size)
        return cache[n];

    /* Compute missing terms using the linear recurrence */
    for (idx_t i = list_size; i <= n; ++i) {
        seq_t term = -cache[i-1];
        term += (seq_t)4 * cache[i-2];
        term -= (seq_t)3 * cache[i-3];
        term +=      cache[i-4];
        cache[i] = term;
    }

    /* Update the stored size */
    list_size = n + 1;
    return cache[n];
}
