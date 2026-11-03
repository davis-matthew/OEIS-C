/*
 * A163349  a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A163349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 984 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* initialize cache with the two base values if not already done */
    if (!cache)
    {
        size_t init = 2;                     /* we need indices 0 and 1 */
        seq_t *tmp = mem_new(seq_t, init);
        if (!tmp) return (seq_t)0;           /* allocation failure, return 0 */

        cache = tmp;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cached_size = 2;
    }

    /* grow cache and compute missing terms if necessary */
    if (n >= cached_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) return (seq_t)0;           /* allocation failure */

        cache = tmp;
        for (idx_t i = cached_size; i <= n; ++i)
        {
            cache[i] = (seq_t)((seq_t)10 * cache[i - 1] - (seq_t)23 * cache[i - 2]);
        }
        cached_size = (idx_t)new_size;
    }

    return cache[n];
}
