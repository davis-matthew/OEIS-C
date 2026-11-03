/*
 * A039318  Numbers whose base-7 representation has the same nonzero number of
 *          1's and 6's.
 * https://oeis.org/A039318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑7 representation has the same nonzero number
 *            of 1’s and 6’s
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether x has equal nonzero counts of 1 and 6 in base 7. */
static bool matches_base7(seq_t x)
{
    unsigned int cnt1 = 0, cnt6 = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        unsigned int d = (unsigned int)(t % (seq_t)7);
        if (d == 1) {
            ++cnt1;
        } else if (d == 6) {
            ++cnt6;
        }
        t /= (seq_t)7;
    }
    return (cnt1 > 0) && (cnt1 == cnt6);
}

/* Return the n-th element of sequence A039318 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* undefined for offset < 1, harmless */
    }

    /* Static cache for previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_val = 1;    /* next integer to test */

    /* Return cached result if available. */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Grow the cache if necessary. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < n) {
            new_cap *= 2;
        }
        seq_t *tmp = (cache_cap == 0) ?
            mem_new(seq_t, new_cap) :
            mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            abort();                /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new terms until we have n entries. */
    while ((size_t)n > cache_len) {
        if (matches_base7(next_val)) {
            cache[cache_len++] = next_val;
        }
        ++next_val;
    }

    return cache[(size_t)n - 1];
}
