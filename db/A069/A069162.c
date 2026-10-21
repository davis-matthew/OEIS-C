/*
 * A069162  a(1)=1, a(2)=2, a(n+2)=(a(n+1)+a(n))/2 if a(n+1)+a(n) is even,
 *          a(n+2)=(3*(a(n+1)+a(n))+1)/2 otherwise.
 * https://oeis.org/A069162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n+2) = (a(n+1)+a(n))/2 if (a(n+1)+a(n)) even,
 *            otherwise a(n+2) = (3*(a(n+1)+a(n))+1)/2
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build. */
static seq_t cache[20000 + 1];
static idx_t cache_size = 0;   /* largest index computed */

seq_t a(idx_t n)
{
    if (n < 1)        /* the OEIS offset is 1; caller never asks for n<1 */
        return 0;

    /* initialise cache on first use */
    if (cache_size == 0) {
        cache[1] = 1;
        cache[2] = 2;
        cache_size = 2;
    }

    /* extend calculation if necessary */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t s = cache[i - 1] + cache[i - 2];
        if ((s % (seq_t)2) == 0)
            cache[i] = s >> 1;
        else
            cache[i] = (s * 3 + 1) >> 1;
    }
    cache_size = n;
    return cache[n];
}
