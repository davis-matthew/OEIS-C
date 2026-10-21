/*
 * A211813  Number of (n+1) X (n+1) -10..10 symmetric matrices with every 2 X
 *          2 subblock having sum zero and two distinct values.
 * https://oeis.org/A211813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) with a(1)=32, a(2)=44, a(3)=62. This
 *            implementation uses memoisation in a static array indexed from 1
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 41, u64 n <= 87, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The calling program supplies the types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Only indices >=1 are valid for this sequence. */
    if (n == 0) return (seq_t)0;

    /* Cache the computed values.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cached_upto = 0;   /* largest index already computed */

    /* Determine the needed array size (gap up to n inclusive). */
    size_t needed = (size_t)n + 1;
    if (needed > cached_upto) {
        /* Grow the array to the required size. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;

        /* Initialise the first three terms if they are not yet present. */
        if (needed >= 2) cache[1] = (seq_t)32;
        if (needed >= 3) cache[2] = (seq_t)44;
        if (needed >= 4) cache[3] = (seq_t)62;

        /* Compute any new terms that have not yet been calculated. */
        size_t start = (cached_upto < 3 ? 4 : cached_upto + 1);
        for (size_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)((seq_t)2 * cache[i - 1] - cache[i - 3]);
        }

        cached_upto = n;
    }

    return cache[(size_t)n];
}
