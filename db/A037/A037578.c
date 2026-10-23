/*
 * A037578  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3.
 * https://oeis.org/A037578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1)+6*a(n-2)+4, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety, offset starts at 1 */

    if (cache == NULL)                   /* first call */
    {
        cache_len = n + 1;               /* need indices 0..n */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = 5 * cache[i-1] + 6 * cache[i-2] + 4;
    }
    else if (cache_len <= n)             /* need to extend cache */
    {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i <= n; ++i)
            cache[i] = 5 * cache[i-1] + 6 * cache[i-2] + 4;
    }

    return cache[n];
}
