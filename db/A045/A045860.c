/*
 * A045860  Numbers whose square has initial digit '6'.
 * https://oeis.org/A045860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose square has initial digit '6'.
   The function searches integers m ≥ 1, computes m² and checks if
   the first decimal digit of m² is 6.  Results are cached for
   subsequent calls.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of found numbers  */
    static size_t cache_len  = 0;      /* how many terms stored   */
    static size_t cache_cap  = 0;      /* allocated capacity      */
    static seq_t last_m       = 0;      /* last m examined         */

    /* Extend the cache until it holds the nth term. */
    while (cache_len < n)
    {
        /* next candidate number */
        seq_t m = last_m + (seq_t)1;
        /* its square */
        seq_t sq = m * m;

        /* number of decimal digits of sq */
        int d = digit_len(sq);             /* 1‑based digit count */

        /* leading digit: divide by 10^(d‑1) */
        seq_t div = sq / pow10((idx_t)(d - 1));

        if (div == (seq_t)6)
        {
            /* grow the cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp)    /* allocation failure – return 0 */
                    return (seq_t)0;
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }

        last_m = m;
    }
    return cache[n - 1];
}
