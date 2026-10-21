/*
 * A218892  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..1 n X 3 array.
 * https://oeis.org/A218892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) + 2*a(n-2) - 5*a(n-3) - 8*a(n-4) - 4*a(n-5)
 * initial values: a(1)=4, a(2)=18, a(3)=72, a(4)=286, a(5)=1162
 */

#include <stddef.h>    /* size_t */
#include <stdlib.h>    /* malloc, realloc */

seq_t a(idx_t n)
{
    /* For an out‑of‑range call (n == 0) return 0.  The tests only
     * use n >= 1, but this keeps the function safe. */
    if (n == 0) return 0;

    /* Static cache to memoise computed terms. cache[0] holds a(1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms already stored */

    /* Return a cached value if we already have it. */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Grow the cache to at least n elements. */
    size_t new_len = (size_t)n;
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;          /* allocation failed – return 0 */
        cache = tmp;
    }

    /* Initial values for the first five terms.  The values are cast to
     * seq_t; they may overflow if seq_t is too small – that is allowed,
     * because the sequence is only required to be exact when it fits
     * in seq_t. */
    static const seq_t init[5] = {
        (seq_t)4,
        (seq_t)18,
        (seq_t)72,
        (seq_t)286,
        (seq_t)1162
    };

    /* Compute missing terms. */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i < 5) {
            cache[i] = init[i];
        } else {
            /* Use only integer arithmetic as required. */
            seq_t term = 4 * cache[i - 1];
            term += 2 * cache[i - 2];
            term -= 5 * cache[i - 3];
            term -= 8 * cache[i - 4];
            term -= 4 * cache[i - 5];
            cache[i] = term;
        }
    }

    cache_len = new_len;
    return cache[(size_t)n - 1];
}
