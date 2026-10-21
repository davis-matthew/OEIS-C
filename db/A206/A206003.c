/*
 * A206003  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one, and every 2 X 2
 *          determinant nonzero.
 * https://oeis.org/A206003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) + 4*a(n-3) for n>4; initial values: a(1)=24,
 *            a(2)=78, a(3)=228, a(4)=722
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index currently cached */

    if (n == 0)                /* undefined for n=0 – return 0 */
        return 0;

    /* Grow cache if necessary */
    if (n > cache_size) {
        idx_t new_len = n + 1;   /* need indices 0 … n (0 unused) */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)         /* allocation failure – unlikely in tests */
            return 0;
        cache = new_cache;

        /* Compute new terms incrementally */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)24;
            else if (i == 2)
                cache[i] = (seq_t)78;
            else if (i == 3)
                cache[i] = (seq_t)228;
            else if (i == 4)
                cache[i] = (seq_t)722;
            else
                cache[i] = (seq_t)8 * cache[i - 2] + (seq_t)4 * cache[i - 3];
        }
        cache_size = n;
    }

    return cache[n];
}
