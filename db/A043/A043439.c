/*
 * A043439  Numbers having three 4's in base 8.
 * https://oeis.org/A043439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   count_fours_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043439: Numbers having exactly three 4's in base 8
 *
 * The algorithm scans the integers in increasing order and
 * keeps those whose base‑8 representation contains exactly
 * three digits equal to 4.  The values are cached so that
 * repeated calls to a(n) are efficient and the sequence
 * can be accessed in any order.
 */

#include <stdlib.h>
#include "../utils/count_fours_base8.h"

static seq_t *cache = NULL;            /* cache of found terms          */
static size_t cache_cap = 0;           /* allocated slots in cache      */
static idx_t  cache_len = 0;           /* number of terms cached        */
static seq_t  last_checked = 0;        /* last integer that was examined*/

/* Grow the cache if needed.  The new capacity is doubled
 * until it is large enough. */
static void expand_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* abort on allocation failure */
    cache   = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined at offset 0 */

    /* Generate terms until we have at least n values cached. */
    while (cache_len < n) {
        last_checked++;            /* move to the next integer */

        if (count_fours_base8(last_checked) == 3) {
            expand_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
