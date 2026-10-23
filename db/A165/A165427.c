/*
 * A165427  a(1) = 1, a(2) = 9, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(2) = 9; a(3) = 9; a(n) = a(n-1)^2 for n >= 4
   (equivalently: a(3) = 9, a(n) = 9^(2^(n-3)) for n >= 3) */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A165427
   (index n >= 1).  The function caches computed values
   so that calls in any order remain fast. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n < 1; behaviour irrelevant */
        return (seq_t)0;

    /* Static cache for already computed terms.
       cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of terms currently stored */

    /* Initialise cache with the first three known terms. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failure – give up */

        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)9;   /* a(2) */
        cache[2] = (seq_t)9;   /* a(3) */
        cache_len = 3;
    }

    /* Extend cache if necessary.  Each new term is the square of the
       previous term (valid for indices >= 4). */
    if (cache_len < n) {
        /* Grow the buffer to hold at least n terms. */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* allocation failure – give up */

        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (idx_t k = cache_len; k < n; ++k) {
            /* k is the zero‑based index of the new term;
               previous term is at index k-1. */
            seq_t prev = cache[k - 1];
            cache[k] = prev * prev;
        }

        cache_len = n;
    }

    return cache[n - 1];
}
