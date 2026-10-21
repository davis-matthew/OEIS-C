/*
 * A235904  Number of (n+1) X (1+1) 0..2 arrays with the minimum plus the
 *          maximum unequal to the lower median plus the upper median in every
 *          2 X 2 subblock.
 * https://oeis.org/A235904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 20*a(n-2) - 8*a(n-3) with a(1) = 48, a(2) =
 *            260, a(3) = 1408
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)                 /* offset is 1 – safety */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (cache_size < n) {
        size_t new_cap = (size_t)(n + 1);                 /* +1 for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                                /* allocation failed */
        cache = tmp;

        /* Initialise the new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)48;
            else if (i == 2)
                cache[i] = (seq_t)260;
            else if (i == 3)
                cache[i] = (seq_t)1408;
            else
                cache[i] = (seq_t)2   * cache[i-1]
                         + (seq_t)20  * cache[i-2]
                         - (seq_t)8   * cache[i-3];
        }
        cache_size = n;
    }

    return cache[n];
}
