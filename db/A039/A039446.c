/*
 * A039446  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 9's.
 * https://oeis.org/A039446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-11 representation has the same strictly positive
 * number of 0's and 9's.
 * Computed by brute‑force search of natural numbers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0U) return 0;      /* guard for illegal indices */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static seq_t  next_candidate = 1;   /* next decimal number to test */

    while (cache_len < n)
    {
        seq_t cand = next_candidate;

        /* count 0's and 9's in base‑11 representation of cand */
        seq_t tmp  = cand;
        seq_t c0   = 0, c9 = 0;
        while (tmp > 0)
        {
            seq_t d = tmp % (seq_t)11;
            if (d == 0)
                ++c0;
            else if (d == 9)
                ++c9;
            tmp /= (seq_t)11;
        }
        if (c0 == c9 && c0 > 0)
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmpp = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                if (!tmpp) exit(EXIT_FAILURE);
                if (cache)
                {
                    memcpy(tmpp, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = tmpp;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
