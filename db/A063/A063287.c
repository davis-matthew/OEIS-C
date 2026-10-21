/*
 * A063287  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          14 ).
 * https://oeis.org/A063287
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) (n > 13) Initial values (offset
 *            2): a(2) = -1, a(3) = 1, a(4) = 4, a(5) = 6, a(6) = 8, a(7) =
 *            10, a(8) = 12, a(9) = 12, a(10) = 16, a(11) = 16, a(12) = 20,
 *            a(13) = 22
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Base values for n = 2 … 13.  base_vals[i] == a(i+2). */
static const seq_t base_vals[12] = {
    -1,   /* a(2) */
     1,   /* a(3) */
     4,   /* a(4) */
     6,   /* a(5) */
     8,   /* a(6) */
    10,   /* a(7) */
    12,   /* a(8) */
    12,   /* a(9) */
    16,   /* a(10) */
    16,   /* a(11) */
    20,   /* a(12) */
    22    /* a(13) */
};

/* Dynamically allocated cache of computed terms.
   cache[i] holds a(i) for all i ≤ cache_sz-1.  Indices 0 and 1 are unused. */
static seq_t *cache   = NULL;
static idx_t  cache_sz = 0;

/* Return the term a(n) for n ≥ 2.  For n < 2 the result is undefined
   (returns 0). */
seq_t a(idx_t n)
{
    if (n < 2)  /* outside the defined offset */
        return 0;

    /* Ensure the cache covers index n. */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;                     /* include index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL)                          /* allocation failed */
            return 0;
        cache = tmp;
        /* Initialise all newly added entries. */
        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i <= 13)                          /* use supplied initial values */
                cache[i] = base_vals[i - 2];
            else                                   /* recurrence for i > 13 */
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
