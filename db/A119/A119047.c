/*
 * A119047  Triangular numbers composed of digits {0,1,9}.
 * https://oeis.org/A119047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000217(A119048(n))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   only_019_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of triangular numbers whose decimal representation
 * uses only the digits 0, 1, and 9.  The function returns the
 * *n*-th such number (with n=1 giving the first term). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/only_019_digits.h"

/* Grow a dynamic array of seq_t values.  The array grows by a factor
 * of two until the desired capacity is reached; this keeps the
 * amortized cost of allocations low and works for both C and C++. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 4 : *cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Return the *n*-th triangular number consisting only of digits
 * from the set {0, 1, 9}.  The function keeps a static cache so
 * that repeated calls are efficient regardless of call order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found terms                  */
    static size_t cache_size = 0;    /* number of terms stored in 'cache'     */
    static size_t cache_cap  = 0;    /* current capacity of 'cache'           */
    static idx_t  current_k   = 1;   /* next k to process (1‑based)           */
    static seq_t current_t   = (seq_t)0; /* T(k-1) for the current value         */

    if (n == 0)
        return (seq_t)0;             /* safety guard – offset is 1             */

    /* Return a cached term if it is already computed. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Generate triangular numbers until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Update T_k = T_{k-1} + k.  current_k starts at 1 so that
         * after the first iteration current_t == T_1. */
        current_t += (seq_t)current_k;
        ++current_k;

        if (only_019_digits(current_t)) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = current_t;
        }
    }

    return cache[n - 1];
}
