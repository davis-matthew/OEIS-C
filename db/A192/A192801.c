/*
 * A192801  Constant term in the reduction of the polynomial (x+2)^n by
 *          x^3->x^2+x+1. See Comments.
 * https://oeis.org/A192801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-15*a(n-2)+11*a(n-3) with a(0) = 1, a(1) = 2,
 *            a(2) = 4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static size_t cached_size = 0;       /* number of elements stored */

seq_t a(idx_t n) {
    /* ensure cache has at least n+1 entries */
    if ((size_t)(n + 1) > cached_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = cached_size == 0
                ? (seq_t*)malloc(new_size * sizeof(seq_t))
                : (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – terminate the program */
            abort();
        }
        cache = new_cache;

        /* compute any missing entries from cached_size up to n */
        for (size_t i = cached_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)4;
            } else {
                /* compute using recurrence:
                   a(i) = 7*a(i-1) + 11*a(i-3) - 15*a(i-2)   (prevent underflow) */
                seq_t term = 7 * cache[i-1] + 11 * cache[i-3] - 15 * cache[i-2];
                cache[i] = term;
            }
        }
        cached_size = new_size;
    }
    return cache[(size_t)n];
}
