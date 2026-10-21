/*
 * A221088  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-3) + a(n-4) with a(1)=0, a(2)=4,
 *            a(3)=24, a(4)=169
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n≥1 */

    static seq_t *cache = NULL;          /* cache[1 .. cache_size] */
    static idx_t cache_size = 0;

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)169;
    }

    /* grow cache if necessary and compute new terms */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t t = (seq_t)7 * cache[i - 1];
            t -= (seq_t)7 * cache[i - 3];
            t += cache[i - 4];
            cache[i] = t;
        }
        cache_size = new_size;
    }

    return cache[n];
}
