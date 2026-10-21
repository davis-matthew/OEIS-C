/*
 * A063147  Dimension of the space of weight 2n cusp forms for Gamma_0( 79 ).
 * https://oeis.org/A063147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) – a(n-4) for n > 5 Initial values:
 *            a(1)=6, a(2)=19, a(3)=33, a(4)=45, a(5)=59
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms currently cached */

    if (n < 1)
        return 0;              /* undefined for n<1, return 0 as safe default */

    /* if the requested term is already cached, return it */
    if (n <= cached)
        return cache[n - 1];

    /* enlarge cache to hold up to term n */
    size_t needed = (size_t)n;
    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!cache)                /* allocation failed – return 0 as safe default */
        return 0;

    /* fill any missing terms, using the recurrence */
    for (size_t i = cached; i < needed; ++i)
    {
        if (i < 5)             /* initialise first five terms (offset 1) */
        {
            switch (i)
            {
                case 0: cache[0] = 6;  break;
                case 1: cache[1] = 19; break;
                case 2: cache[2] = 33; break;
                case 3: cache[3] = 45; break;
                case 4: cache[4] = 59; break;
            }
        }
        else                    /* recurrence: a(n) = a(n-1)+a(n-3)-a(n-4) */
        {
            /* indices: i = n-1 */
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
    }

    cached = needed;
    return cache[n - 1];
}
