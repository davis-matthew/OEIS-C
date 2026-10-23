/*
 * A147537  Numbers whose binary representation is the concatenation of 2n-1
 *          digits 1 and n digits 0.
 * https://oeis.org/A147537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^(n-1)*(4^n - 2). Recurrence: a(1)=2, a(2)=28,
 *            a(n)=10*a(n-1)-16*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= cache_max) return cache[n-1];
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    cache = tmp;
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[0] = (seq_t)2;
        else if (i == 2)
            cache[1] = (seq_t)28;
        else
            cache[i-1] = (seq_t)((seq_t)10 * cache[i-2] - (seq_t)16 * cache[i-3]);
    }
    cache_max = n;
    return cache[n-1];
}
