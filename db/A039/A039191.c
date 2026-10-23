/*
 * A039191  Numbers whose base-11 representation has the same number of 4's
 *          and 7's.
 * https://oeis.org/A039191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of integers whose base‑11 representation has the
 *            same number of 4's and 7's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (so a(1) is the first term listed below) */

#include <stddef.h>
#include "../utils/grow_seq.h"

/* static storage for memoization */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 0;

/* ensures that cache contains at least n entries (n >= 1) */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        seq_t val = next_candidate;          /* candidate number to test */
        next_candidate++;                    /* advance for next call */

        /* count 4's and 7's in base‑11 representation */
        seq_t x = val;
        int cnt4 = 0, cnt7 = 0;
        if (x == 0) /* 0 has no 4's or 7's; counts remain 0 */
        {
        }
        else
        {
            while (x > 0)
            {
                int digit = (int)(x % 11);
                if (digit == 4)  cnt4++;
                else if (digit == 7) cnt7++;
                x /= 11;
            }
        }

        /* add to cache if balanced */
        if (cnt4 == cnt7)
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = val;
            cache_len++;
        }
    }
}

seq_t a(idx_t n)
{
    /* The offset is 1, so a(1) == cache[0] */
    if (n == 0)          /* undefined, but return 0 for safety */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n - 1];
}
