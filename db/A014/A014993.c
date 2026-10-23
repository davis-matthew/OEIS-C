/*
 * A014993  a(n) = (1 - (-11)^n)/12.
 * https://oeis.org/A014993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-11)^n)/12
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 19, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the linear recurrence a(n) = -10*a(n-1) + 11*a(n-2),
   with initial values a(1) = 1, a(2) = -10.  The result is stored in a
   statically allocated cache so subsequent calls are O(1) for already
   computed indices, and O(n) for new ones. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;   /* highest n computed (1‑based) */
    static size_t cap = 0;          /* allocated size */

    if (n == 0) {
        return 0; /* undefined for 0, but return 0 as safe value */
    }

    /* initial allocation with the first two terms */
    if (!cache) {
        cap = (n < 2 ? 2 : n);
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(1) */
        cache[1] = (seq_t)-10;    /* a(2) */
        cached_upto = 2;
    }
    else if (n > cap) {
        /* grow cache to accommodate n */
        cap = n;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* if already computed, return directly */
    if (n <= cached_upto) {
        return cache[n - 1];
    }

    /* compute missing values up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 2]; /* a(i-1) */
        seq_t prev2 = cache[i - 3]; /* a(i-2) */
        cache[i - 1] = (seq_t)-10 * prev1 + (seq_t)11 * prev2;
    }
    cached_upto = n;
    return cache[n - 1];
}
