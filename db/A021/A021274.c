/*
 * A021274  Expansion of g.f. 1/((1 - x)*(1 - 2*x)*(1 - 8*x)*(1 - 11*x)).
 * https://oeis.org/A021274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*11^(n+3) - 15*8^(n+3) + 35*2^(n+3) - 27)/1890.
 *            Recurrence: a(0)=1, a(1)=22, a(2)=337, a(3)=4482,
 *            a(n)=22*a(n-1)-147*a(n-2)+302*a(n-3)-176*a(4-n) for n>3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t cache_size = 0; /* number of cached entries */

    size_t idx = (size_t)n;

    /* If already cached, return the value immediately. */
    if (idx < cache_size)
        return cache[idx];

    /* Extend the cache to hold n+1 entries. */
    size_t new_size = idx + 1;
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

    /* Compute any new entries.  cache_size contains the first unused index. */
    for (size_t i = cache_size; i <= idx; ++i)
    {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 22;
        else if (i == 2)
            cache[i] = 337;
        else if (i == 3)
            cache[i] = 4482;
        else
        {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            /* Recurrence: a(n)=22*a(n-1)-147*a(n-2)+302*a(n-3)-176*a(n-4) */
            cache[i] = 22*a1 - 147*a2 + 302*a3 - 176*a4;
        }
    }
    cache_size = new_size;
    return cache[idx];
}
