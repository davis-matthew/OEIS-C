/*
 * A084707  a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4) for n > 3, with a(0) = 1, a(1)
 *          = 3, a(2) = 9, a(3) = 27.
 * https://oeis.org/A084707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4), a(0)=1, a(1)=3, a(2)=9,
 *            a(3)=27 (OEIS sequence A084707)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static idx_t cache_len = 0;   /* number of cached elements */

    /* return already computed value */
    if (n < cache_len)
        return cache[n];

    /* grow cache to size n+1 */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)   /* allocation failed – give up safely */
        return (seq_t)0;
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)        cache[i] = (seq_t)1;
        else if (i == 1)   cache[i] = (seq_t)3;
        else if (i == 2)   cache[i] = (seq_t)9;
        else if (i == 3)   cache[i] = (seq_t)27;
        else
            cache[i] = 3 * cache[i-1] - 3 * cache[i-3] + cache[i-4];
    }

    cache_len = new_len;
    return cache[n];
}
