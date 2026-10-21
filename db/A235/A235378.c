/*
 * A235378  a(n) = (-1)^n*(n! - (-1)^n).
 * https://oeis.org/A235378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A235378
 *  Formula: a(n) = (-1)^n * (n! - (-1)^n)
 *  Recurrence (used in the implementation):
 *      a(1) = -2
 *      a(2) =  1
 *      a(n) = -n * a(n-1) - n - 1   for n > 2
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation table (1‑based values stored in a zero‑based array) */
    static seq_t *cache = NULL;   /* cache[0] contains a(1) */
    static size_t cap   = 0;      /* allocated capacity (number of slots) */
    static size_t size  = 0;      /* highest n that has already been computed */

    if (n < 1) return 0;         /* undefined for indices below the offset */

    /* If a(n) has not yet been computed, extend cache up to n and fill it */
    if ((size_t)n > size) {
        /* Determine a new capacity at least the requested size */
        size_t newcap = cap;
        if (newcap == 0) newcap = 8;
        while (newcap < (size_t)n) newcap <<= 1;

        /* Reallocate (NULL is acceptable for realloc) */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) abort();     /* memory allocation failed */

        cap = newcap;

        /* Compute the missing terms from size+1 up to n */
        for (idx_t i = size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)-2;          /* a(1) */
            } else if (i == 2) {
                cache[1] = (seq_t)1;           /* a(2) */
            } else {
                seq_t ii   = (seq_t)i;          /* cast to seq_t only once */
                seq_t prev = cache[i - 2];      /* a(i-1)   (0‑based index) */
                seq_t prod = ii * prev;         /* i * a(i-1) */
                cache[i - 1] = -prod - ii - (seq_t)1;
            }
        }

        size = n;                       /* highest n now computed */
    }

    return cache[n - 1];                /* indices are 1‑based in the API */
}
