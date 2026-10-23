/*
 * A043383  Numbers having three 3's in base 6.
 * https://oeis.org/A043383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_three_threes_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly three 3's in base‑6 representation. */
#include <stdlib.h>
#include "../utils/has_three_threes_in_base6.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    while (cache_len < n)
    {
        /* next integer to test */
        seq_t cand = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;

        /* find the next number with exactly three 3's */
        while (!has_three_threes_in_base6(cand))
            cand++;

        /* grow cache if necessary */
        if (cache_len + 1 > cache_cap)
        {
            idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            while (new_cap < cache_len + 1)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
