/*
 * A176370  x-values in the solution to x^2 - 66*y^2 = 1.
 * https://oeis.org/A176370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 130*a(n-1) - a(n-2), a(1)=1, a(2)=65
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* n is 1‑based. Guard against 0 for safety. */
    if (n == 0)
        return (seq_t)0;          /* undefined, but never called */

    /* Convert idx_t to size_t for array indices; n will be < 10000 in tests. */
    size_t nn = (size_t)n;

    static seq_t *cache = NULL;   /* stores a(1) at cache[0] */
    static size_t cap   = 0;      /* current allocated capacity */
    static size_t used  = 0;      /* number of terms currently computed */

    /* Allocate or reallocate the cache if needed. */
    if (cache == NULL) {
        size_t init = (nn > 2 ? nn : 2);
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        cap   = init;
    } else if (nn > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, nn * sizeof(seq_t));
        if (tmp != NULL) {                /* normal reallocation */
            cache = tmp;
            cap   = nn;
        } else {                          /* allocation failure: fall back to allocate anew */
            cache = (seq_t*)malloc(nn * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0;
            cap   = nn;
        }
    }

    /* Ensure the first two terms are present. */
    if (used < 2) {
        if (used == 0)
            cache[0] = (seq_t)1;          /* a(1) */
        if (used < 2)
            cache[1] = (seq_t)65;        /* a(2) */
        used = 2;
    }

    /* Compute any missing terms up to nn using the recurrence. */
    for (size_t i = used; i < nn; ++i) {
        cache[i] = (seq_t)130 * cache[i - 1] - cache[i - 2];
    }
    used = nn;

    return cache[nn - 1];
}
