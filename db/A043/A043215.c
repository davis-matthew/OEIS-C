/*
 * A043215  Numbers k such that 7 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Sequence  A043215 – Numbers k whose base‑9 representation contains
 *        the adjacent substring “78” or “87”, but (k−1) does not.
 *
 * This implementation enumerates integers on demand, caching the
 * results.  For a given n it scans successive integers until the
 * n‑th qualifying number has been found and then returns it.  The
 * algorithm is O(n) in the size of the output and easily handles
 * n up to ~10⁴ (more if required).
 *********************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff x's base‑9 digits contain 7 followed by 8 or
 * 8 followed by 7 as adjacent digits. */
static bool has_adjacent78(seq_t x)
{
    if (x == (seq_t)0)         /* no digits */
        return false;

    const seq_t nine = (seq_t)9;
    const seq_t d7   = (seq_t)7;
    const seq_t d8   = (seq_t)8;

    seq_t prev = x % nine;     /* least‑significant digit */
    x /= nine;
    while (x != (seq_t)0)
    {
        seq_t cur = x % nine;
        if ((prev == d7 && cur == d8) || (prev == d8 && cur == d7))
            return true;
        prev = cur;
        x /= nine;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already discovered terms. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* storage capacity of cache */
    static idx_t   count = 0;   /* how many terms are stored */
    static seq_t   last_checked = (seq_t)0;  /* last integer examined */

    /* Produce terms up to requested index. */
    while (count < n)
    {
        /* Inspect next integer. */
        last_checked += (seq_t)1;
        seq_t k = last_checked;

        if (has_adjacent78(k) && !has_adjacent78(k - (seq_t)1))
        {
            /* Ensure enough space in the cache. */
            if (count >= cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL)
                    abort();           /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = k;
        }
    }

    return cache[n - (idx_t)1];
}
