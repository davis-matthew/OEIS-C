/*
 * A048776  First partial sums of A048739; second partial sums of A000129.
 * https://oeis.org/A048776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(1) = 4 a(n) = 2*a(n-1) + a(n-2) + n + 1 for n >= 2
 *            (OEIS: first partial sums of A048739; second partial sums of
 *            A000129) The implementation uses memoisation with an expanding
 *            array so that any call to a(idx_t n) runs in O(n) time overall.
 *            The values are computed directly in the type se...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t cached_size = 0; /* number of entries in cache */

    if (n + 1 > cached_size) {    /* need to extend the cache */
        idx_t new_size = n + 1;

        /* allocate / grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – let the program abort */
            exit(1);
        }
        cache = tmp;

        /* initialise the base cases if they have not been set */
        if (cached_size == 0) {
            cache[0] = (seq_t)1;
            cached_size = 1;
        }
        if (cached_size <= 1 && new_size >= 2) {
            cache[1] = (seq_t)4;
            cached_size = 2;
        }

        /* compute new values up to n */
        for (idx_t i = cached_size; i <= n; ++i) {
            /* i >= 2 here, because the base cases are already present */
            cache[i] = 2 * cache[i-1] + cache[i-2] + (seq_t)i + 1;
        }
        cached_size = new_size;
    }
    return cache[n];
}
