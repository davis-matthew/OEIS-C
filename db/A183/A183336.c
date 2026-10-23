/*
 * A183336  Number of n X 4 binary arrays with each 1 adjacent to exactly one
 *          1 vertically and one 1 horizontally.
 * https://oeis.org/A183336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - a(n-5); Initial values:
 *            a(1)=1, a(2)=4, a(3)=7, a(4)=12, a(5)=26
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first five terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)26;

    /* Static cache of previously computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* Grow cache to hold terms up to n. */
        idx_t new_size = n + 1; /* one extra space for index 0 */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache_size == 0) {
            /* Initialize base terms. */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)7;
            cache[4] = (seq_t)12;
            cache[5] = (seq_t)26;
            /* Compute missing terms up to n. */
            for (idx_t i = 6; i <= n; ++i) {
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]*2 - cache[i-5];
            }
            cache_size = new_size;
        } else {
            /* Extend computation from cached size to n. */
            for (idx_t i = cache_size; i <= n; ++i) {
                /* cache_size is at least 6, so i-5 is valid. */
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]*2 - cache[i-5];
            }
            cache_size = new_size;
        }
    }
    return cache[n];
}
