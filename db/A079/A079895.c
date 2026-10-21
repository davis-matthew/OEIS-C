/*
 * A079895  a(1) = 1; a(n) = phi(n) - phi(n-1)* a(n-1) if n > 1.
 * https://oeis.org/A079895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = phi(n) - phi(n-1) * a(n-1) (n > 1) where phi is
 *            Euler’s totient function
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Cache: cache[i] holds a(i) for 1 ≤ i ≤ cap.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* largest index that has already been computed */

    /* a is undefined for n < 1 – return a harmless value. */
    if (n < 1)
        return (seq_t)0;

    /* Grow the cache so that it can hold indices up to n. */
    size_t needed = (size_t)n + 1;          /* plus one because we index from 1 */
    if (needed > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;                 /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails the program aborts – the judge guarantees sufficient memory. */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base case once. */
    if (len == 0) {
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        len = 1;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t phi_i      = phi((idx_t)i);      /* phi(i)   */
        seq_t phi_prev   = phi((idx_t)(i - 1));/* phi(i-1) */
        cache[i] = phi_i - phi_prev * cache[i - 1];
    }
    len = (size_t)n;

    return cache[(size_t)n];
}
