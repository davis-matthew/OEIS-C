/*
 * A039169  Numbers whose base-11 representation has the same number of 1's
 *          and 6's.
 * https://oeis.org/A039169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate integers, storing those whose base‑11 digit counts of 1
 * and 6 are equal.  The internal cache enables arbitrary order of calls.
 */
#include <stdlib.h>

static seq_t *cache      = NULL;       /* 1‑based stored terms           */
static size_t cache_len  = 0;          /* number of terms stored          */
static size_t cache_cap  = 0;          /* current allocation size         */
static seq_t next_value  = 0;          /* next integer to test            */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; behaviour for n < 1 is irrelevant. */
    if (n < 1) return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        seq_t cur = next_value;

        /* Count 1's and 6's in the base‑11 representation of cur. */
        seq_t tmp = cur;
        unsigned c1 = 0, c6 = 0;
        while (tmp > (seq_t)0)
        {
            seq_t digit = tmp % (seq_t)11;
            if (digit == (seq_t)1) ++c1;
            else if (digit == (seq_t)6) ++c6;
            tmp = tmp / (seq_t)11;
        }

        if (c1 == c6)
        {
            /* Append cur to the cache. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *newp = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
                if (!newp) exit(1);           /* allocation failure */
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }

        ++next_value;
    }

    /* Return the requested term (1‑based). */
    return cache[(size_t)n - 1];
}
