/*
 * A044355  Numbers n such that string 2,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has23, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for numbers whose decimal representation contains the substring
   "23" but whose predecessor does not, and caches the results for later
   calls.  The function works for any seq_t that supports the required
   arithmetic operations. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/has23.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cur_checked = 1;       /* next candidate to examine */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset starts at 1; undefined for 0, return 0 */
        return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next qualifying number. */
        while (true)
        {
            if (has23(cur_checked) && !has23(cur_checked - 1))
            {
                /* Ensure sufficient capacity. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cur_checked;
                cur_checked++; /* advance for the next search */
                break;
            }
            cur_checked++;
        }
    }
    return cache[n - 1];
}
