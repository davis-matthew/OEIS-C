/*
 * A097068  a(n)=Sum(C(n,2k+1)5^k 3^(2k+1) 7^(n-2k-1), k=0,..,Floor[(n-1)/2]).
 * https://oeis.org/A097068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=3, a(n)=14*a(n-1)-4*a(n-2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size)
    {
        /* enlarge cache */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* on allocation failure return 0 */
        cache = new_cache;

        /* initialise new entries */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
                cache[i] = 0;
            else if (i == 1)
                cache[i] = 3;
            else
                cache[i] = 14 * cache[i - 1] - 4 * cache[i - 2];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
