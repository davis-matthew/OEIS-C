/*
 * A094649  An accelerator sequence for Catalan's constant.
 * https://oeis.org/A094649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, a(1)=1, a(2)=7,
   a(n)=3*a(n-2)+a(n-3)-3   for n≥3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[i] holds a(i) for 0 ≤ i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* If n is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold values up to n. */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) /* allocation failed – give up */
        return (seq_t)0;
    cache = tmp;

    /* Fill the cache from the current size up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)4;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)7;
        else
            cache[i] = (3 * cache[i - 2]) + cache[i - 3] - (seq_t)3;
    }

    cache_len = n + 1;
    return cache[n];
}
