/*
 * A024743  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=3, a(2)=1, and a(3)=2.
 * https://oeis.org/A024743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n+1)/4⌋} a(2*i-1) * a(n-2*i+1), with a(1)=3,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   ensure_cache, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include "../utils/ensure_cache.h"   /* ensure_cache() */
#include "../utils/ensure_capacity.h"   /* ensure_capacity() */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* static array to remember which indices are already initialized */
    static uint8_t *init = NULL;
    static size_t init_cap = 0;
    static size_t done = 0;   /* largest index whose value is known */

    /* Ensure memory for both arrays */
    ensure_cache(n, &cache, &cache_cap);

    size_t old_init_cap = init_cap;
    ensure_capacity(n + 1, &init_cap, (void **)&init, sizeof(uint8_t));
    /* Zero the newly allocated part of init array */
    for (size_t i = old_init_cap; i <= n; ++i)
        init[i] = 0;

    /* Compute missing values in increasing order */
    for (idx_t k = done + 1; k <= n; ++k)
    {
        if (k == 1)
            cache[1] = (seq_t)3;
        else if (k == 2)
            cache[2] = (seq_t)1;
        else if (k == 3)
            cache[3] = (seq_t)2;
        else
        {
            seq_t sum = 0;
            idx_t m = (k + 1) / 4;          /* ⌊(k+1)/4⌋ */
            for (idx_t i = 1; i <= m; ++i)
            {
                idx_t j1 = 2 * i - 1;
                idx_t j2 = k - 2 * i + 1;
                sum += cache[j1] * cache[j2];
            }
            cache[k] = sum;
        }
        init[k] = 1;
    }
    done = n;

    return cache[n];
}
