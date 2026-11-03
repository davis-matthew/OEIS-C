/*
 * A044026  Numbers k such that 4 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for numbers k that contain the adjacent digits "45" or "54"
   in their decimal representation, but whose successor k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* true if x has the pattern 45 or 54 in its decimal digits */
static bool has45or54(seq_t x)
{
    while (x >= 10) {
        int low  = (int)(x % 10);   /* least significant digit */
        x      /= 10;               /* drop the low digit */
        int high = (int)(x % 10);   /* digit to the left of low */
        if ((high == 4 && low == 5) || (high == 5 && low == 4))
            return true;
    }
    return false;
}

/* Return the n-th term of A044026 (offset 1). */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;
    static seq_t last_checked = 0;   /* largest k tested so far */

    if (n == 0)            /* undefined for offset 1 – guard anyway */
        return 0;

    /* Build cache until we have at least n entries. */
    while (cache_len < n) {
        seq_t k = ++last_checked;   /* candidate number */

        if (has45or54(k) && !has45or54(k + 1)) {
            /* store k in cache */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache     = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
