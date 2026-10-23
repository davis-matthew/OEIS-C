/*
 * A135360  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) for n > 4, with first terms
 *          1, 2, 4, 7.
 * https://oeis.org/A135360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135360
   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3)  for n > 4
   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 7, a(4) = 12
   The recurrence holds for n ≥ 5.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of elements in cache */

    /* Ensure capacity for at least n+1 elements, but always at least 5 */
    idx_t needed = n + 1;
    if (needed < 5) needed = 5;

    if (cache_size < needed) {
        /* Allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – use static allocation as a fallback */
            tmp = (seq_t *)malloc(needed * sizeof(seq_t));
            if (!tmp) exit(1);
        }
        cache = tmp;

        /* Initialise immutable first five terms only once */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)7;
            cache[4] = (seq_t)12;
            cache_size = 5;
        }

        /* Compute any new terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Recurrence valid only for i >= 5 */
            cache[i] = 4 * cache[i-1] - 6 * cache[i-2] + 4 * cache[i-3];
        }
        cache_size = needed;
    }

    return cache[n];
}
