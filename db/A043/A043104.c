/*
 * A043104  Numbers k such that 1 and 3 occur juxtaposed in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A043104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 1 and 3 occur juxtaposed in the base-4 representation of k but not of k-1.
   Computed by linear scan with caching. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    while (cache_len < (size_t)n)
    {
        idx_t k = next_k;
        bool has = false;
        bool has_prev = false;

        /* check k */
        idx_t t = k;
        if (t != 0) {
            idx_t prev = t % 4;
            t /= 4;
            while (t != 0)
            {
                idx_t cur = t % 4;
                if ((prev == 1 && cur == 3) || (prev == 3 && cur == 1))
                    has = true;
                prev = cur;
                t /= 4;
            }
        }

        /* check k-1 */
        if (k > 0) {
            idx_t u = k - 1;
            if (u != 0) {
                idx_t prev2 = u % 4;
                u /= 4;
                while (u != 0)
                {
                    idx_t cur2 = u % 4;
                    if ((prev2 == 1 && cur2 == 3) || (prev2 == 3 && cur2 == 1))
                        has_prev = true;
                    prev2 = cur2;
                    u /= 4;
                }
            }
        }

        if (has && !has_prev)
        {
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                while (new_cap < cache_len + 1) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        next_k++;
    }
    return cache[(size_t)n - 1];
}
