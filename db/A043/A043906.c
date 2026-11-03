/*
 * A043906  Numbers k such that 2 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k with the digits 2 and 4 occurring adjacent in the
 * base‑6 representation of k (in either order), but not in that of k+1.
 * This code lazily builds a cache of the sequence terms.            */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Returns true iff x (in base‑6) contains a pair of adjacent digits
 * 2 and 4 in either order.  x>0. */
static bool has_adj_24(seq_t x)
{
    if (x == 0)
        return false;                   /* base‑6 "0" has no digits 2 or 4 */

    /* Base‑6 digits stored little‑endian: d[0] = least significant digit. */
    uint8_t d[128];
    int len = 0;
    seq_t tmp = x;
    while (tmp != 0)
    {
        d[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }

    /* Scan for a 2 followed by a 4 or a 4 followed by a 2. */
    for (int i = len - 1; i > 0; --i)
    {
        if ((d[i] == 2 && d[i - 1] == 4) ||
            (d[i] == 4 && d[i - 1] == 2))
            return true;
    }
    return false;
}

/* Return the n‑th element of the sequence, for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                             /* offset handling: ignored */

    /* Static cache of already computed terms.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;             /* number of terms stored */
    static seq_t next_candidate = 1;          /* integer to test next */

    /* Grow cache if needed. */
    if (cache_count < n)
    {
        size_t new_cap = cache_count ? cache_count * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort();                          /* allocation failure */
        cache = tmp;

        /* Compute terms until we have at least n of them. */
        while (cache_count < n)
        {
            /* Find the next candidate that satisfies the definition. */
            while (true)
            {
                if (has_adj_24(next_candidate) &&
                    !has_adj_24(next_candidate + 1))
                {
                    cache[cache_count] = next_candidate;
                    cache_count++;
                    next_candidate++;           /* start search after this k */
                    break;
                }
                next_candidate++;
            }
        }
    }
    return cache[n - 1];
}
