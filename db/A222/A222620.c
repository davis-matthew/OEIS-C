/*
 * A222620  Palindromic time display in hours, minutes, seconds on a six
 *          spaced 24-hour digital clock.
 * https://oeis.org/A222620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 454, u32 n <= 699, u64 n <= 699,
 *            u128 n <= 699, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic time display ignoring leading zeros:
   a(n) = the n-th smallest integer HHMMSS with
   0 ≤ HH ≤ 23, 0 ≤ MM,SS < 60 such that the decimal
   representation of HHMMSS, after removing leading zeros,
   is a palindrome. The first 24 terms are listed in the OEIS entry. */

#include <stddef.h>
#include "../utils/append_candidate.h"

/* static cache of all palindromic times (max 699 entries) */
static seq_t *cache = NULL;      /* array of results */
static size_t cache_len = 0;     /* number of stored values */

/* generate cache on first use */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* build array using append_candidate (grows automatically) */
    seq_t *tmp = NULL;
    size_t len = 0;
    size_t cap = 0;

    for (uint64_t h = 0; h < 24; ++h)
    {
        for (uint64_t m = 0; m < 60; ++m)
        {
            for (uint64_t s = 0; s < 60; ++s)
            {
                /* compute integer value HHMMSS */
                seq_t val = (seq_t)h * 10000 + (seq_t)m * 100 + (seq_t)s;

                /* reverse val (base 10) */
                seq_t rev = 0;
                seq_t t = val;
                while (t > 0)
                {
                    rev = rev * 10 + (t % 10);
                    t = t / 10;
                }

                /* special case: val == 0 */
                if (val == 0)
                    rev = 0;

                /* palindrome check */
                if (val == rev)
                {
                    append_candidate(&tmp, &len, &cap, val);
                }
            }
        }
    }
    cache = tmp;
    cache_len = len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_cache();
    if ((idx_t)(cache_len) < n)
        return 0;   /* out of bounds; sequence ends after 699 terms */

    return cache[n - 1];
}
