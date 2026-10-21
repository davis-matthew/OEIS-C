/*
 * A052954  Expansion of (2-x-x^2-x^3)/((1-x)*(1-x^2-x^3)).
 * https://oeis.org/A052954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 42, u32 n <= 82, u64 n <= 160,
 *            u128 n <= 318, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052954
    Recurrence:  a(n) = a(n-2) + a(n-3) - 1   for n ≥ 3
    Initial terms:  a(0) = 2,  a(1) = 1,  a(2) = 2   (offset 0) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the three initial values directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Static cache that survives between calls and can grow as needed. */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;     /* allocated array size */
    static idx_t   max_cal = 2;     /* highest index that has been computed */

    /* Ensure the cache is big enough for index n. */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (cap == 0 ? 4 : cap);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* simple error handling */
        cache = tmp;
        cap   = new_cap;

        /* Initialise the first three known terms. */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;

        /* Ensure the bookkeeping value is correct. */
        if (max_cal < 2) max_cal = 2;
    }

    /* Compute any missing terms up to n (increasing order). */
    if (max_cal < n) {
        for (idx_t i = max_cal + 1; i <= n; ++i) {
            cache[(size_t)i] =
                cache[(size_t)(i-2)] + cache[(size_t)(i-3)] - (seq_t)1;
        }
        max_cal = n;
    }

    return cache[(size_t)n];
}
