/*
 * A229581  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 36*a(n-2); a(1)=2, a(2)=28
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* enlarge cache to size n */
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory – fall back to zero */
            if (cache) free(cache);
            return 0;
        }
        cache = new_cache;
        cache_size = n;

        /* initialize first two terms */
        if (n >= 1) cache[0] = (seq_t)2;
        if (n >= 2) cache[1] = (seq_t)28;

        /* fill remaining terms using recurrence */
        for (idx_t i = 2; i < n; ++i)
            cache[i] = ((seq_t)12 * cache[i-1]) - ((seq_t)36 * cache[i-2]);
    }

    return cache[n-1];
}
