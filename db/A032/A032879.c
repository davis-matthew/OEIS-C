/*
 * A032879  Numbers whose that base-8 representation Sum_{i=0..m} d(i)*8^i has
 *          d(0) > d(1) < d(2) > ...
 * https://oeis.org/A032879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches increasing integers for alternating base‑8 digits   */
/* pattern: least significant digit > next > next < next ...    */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_test  = 1;   /* next integer to test */

    while (cache_len < n)
    {
        uint64_t x = next_test;
        bool     ok = true;
        int      prev = -1;         /* previous digit (more significant) */
        int      parity = 0;        /* 0 for d0, 1 for d1, ... */

        uint64_t y = x;
        while (y > 0)
        {
            int d = (int)(y & 7ULL);   /* y % 8 */

            if (prev != -1)
            {
                if ((parity & 1) == 0)          /* even index: d > prev */
                {
                    if (!(d > prev))
                    {
                        ok = false;
                        break;
                    }
                }
                else                              /* odd index: d < prev */
                {
                    if (!(d < prev))
                    {
                        ok = false;
                        break;
                    }
                }
            }

            prev = d;
            y >>= 3;                      /* y /= 8 */
            ++parity;
        }

        if (ok)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* memory error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
        }

        ++next_test;
    }

    return cache[n - 1];
}
