/*
 * A231307  Recurrence a(n) = a(n-2) + n^M for M=8, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + n^8, a(0)=0, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 189, u128 n <= 9999,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t pow8(seq_t base)
{
    seq_t r = 1;
    for (int j = 0; j < 8; ++j)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache_size <= n)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failure – undefined behaviour OK */

        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = 0;
            else if (i == 1)
                cache[1] = 1;
            else
                cache[i] = cache[i - 2] + pow8((seq_t)i);
        }
        cache_size = new_size;
    }

    return cache[n];
}
