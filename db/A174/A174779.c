/*
 * A174779  y-values in the solution to x^2 - 42*y^2 = 1.
 * https://oeis.org/A174779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - a(n-2), with a(1) = 0, a(2) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t len    = 0; /* number of terms currently stored */

    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* ensure cache has space for at least n terms */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 2;   /* double, or start at 2 */
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base terms if not already done */
    if (len < 2) {
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        len = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i < n; ++i) {
        cache[i] = (seq_t)(26) * cache[i - 1] - cache[i - 2];
    }
    len = n;
    return cache[n - 1];
}
