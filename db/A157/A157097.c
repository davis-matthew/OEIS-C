/*
 * A157097  Consider all Consecutive Integer Pythagorean 11-tuples (X, X+1,
 *          X+2, X+3, X+4, X+5, Z-4, Z-3, Z-2, Z-1, Z) ordered by increasing
 *          Z; sequence gives Z values.
 * https://oeis.org/A157097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned, easy
 * Formula:   a(n) = 22*a(n-1) - a(n-2) - 40 for n > 1; * a(0) = 5, a(1) = 65
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that expands lazily and works for unordered calls. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity (in elements) */
    static size_t len = 0;   /* number of values currently stored (valid indices 0 .. len-1) */

    /* Ensure the cache can hold index n. */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* Initialise the first two terms if necessary. */
    if (len == 0) {
        cache[0] = 5;
        len = 1;
    }
    if (len == 1) {
        cache[1] = 65;
        len = 2;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* For built‑in integer types the operations may overflow,
         * which is acceptable: the results are only required to be
         * exact when they fit in seq_t. For arbitrary‑precision types
         * the operators are overloaded to perform exact arithmetic. */
        cache[i] = cache[i-1] * 22 - cache[i-2] - 40;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
