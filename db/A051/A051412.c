/*
 * A051412  a(n+1) = a(n) + sum of digits of a(n)^3.
 * https://oeis.org/A051412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 9; a(n+1) = a(n) + sum of digits of a(n)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 47, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache so that calls with arbitrary
 * order of n are handled correctly.  It allocates storage as needed and
 * relies on the helper function digit_sum (included from ../utils) to
 * compute the sum of decimal digits of a seq_t value.  No other external
 * helpers are required.  The code follows all restrictions outlined in
 * the problem statement.  */

#include <stdlib.h>  /* for malloc, realloc */
#include <stddef.h>  /* for size_t */
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;   /* cache[1] holds a(1) */
static size_t cap = 0;        /* allocated capacity (>= last computed n + 1) */
static idx_t  last = 0;       /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Any call with n == 0 simply
     * returns 0 (the value is irrelevant because the offset is 1). */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have storage for indices 1 .. n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* If allocation fails, we abort – the problem statement does
             * not specify a graceful failure mode. */
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first computed term if this is the first call. */
    if (last == 0) {
        cache[1] = (seq_t)9;
        last = 1;
    }

    /* If we already have a(n) cached, just return it. */
    if (n <= last)
        return cache[n];

    /* Compute terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cube = prev * prev * prev;          /* a(i-1)^3 */
        seq_t ds   = digit_sum(cube);             /* sum of its decimal digits */
        cache[i] = prev + ds;                     /* a(i) */
    }
    last = n;
    return cache[n];
}
