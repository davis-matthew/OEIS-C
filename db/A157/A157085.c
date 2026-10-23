/*
 * A157085  Consider all Consecutive Integer Pythagorean quintuples (X, X+1,
 *          X+2, Z-1, Z) ordered by increasing Z; sequence gives Z values.
 * https://oeis.org/A157085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 14, for n > 1 : a(n) = 10*a(n-1) - a(n-2) - 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size */
    static idx_t  max_n = 0;          /* largest n computed so far (0 means none) */

    /* Grow the cache if necessary. */
    if (n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base terms if they are not yet set. */
    cache[0] = (seq_t)2;
    if (n >= 1) cache[1] = (seq_t)14;

    /* Compute only the missing part. */
    idx_t start = (max_n >= 2) ? max_n + 1 : 2;   /* 2 is the first index that needs recurrence */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)10 * cache[i - 1]
                   - cache[i - 2]
                   - (seq_t)4;
    }

    if (n > max_n) max_n = n;

    return cache[n];
}
