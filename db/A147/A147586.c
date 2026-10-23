/*
 * A147586  a(n) = A142710(n)/2.
 * https://oeis.org/A147586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 12*a(n-3) - 11*a(n-4) + 16*a(n-5) -
 *            4*a(n-6), for n >= 7; a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=19,
 *            a(5)=56, a(6)=138
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len <= n)
    {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p)
            exit(EXIT_FAILURE);            /* memory allocation failed */
        cache = p;

        for (idx_t i = old_len; i < new_len; ++i)
        {
            switch (i)
            {
            case 0: cache[i] = 1; break;
            case 1: cache[i] = 1; break;
            case 2: cache[i] = 3; break;
            case 3: cache[i] = 7; break;
            case 4: cache[i] = 19; break;
            case 5: cache[i] = 56; break;
            case 6: cache[i] = 138; break;
            default:
                {
                    seq_t term = 2 * cache[i - 1]
                               + 7 * cache[i - 2]
                               - 12 * cache[i - 3]
                               - 11 * cache[i - 4]
                               + 16 * cache[i - 5]
                               - 4 * cache[i - 6];
                    cache[i] = term;
                }
                break;
            }
        }
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
