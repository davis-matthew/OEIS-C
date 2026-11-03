/*
 * A043925  Numbers k such that 2 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k whose base‑7 representation contains the adjacent
   pair “26” or “62” but k+1 does not.  a(1) = 20. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑7 representation of *x contains an
   adjacent pair 26 or 62. */
static bool has_adj26(const seq_t *x)
{
    seq_t tmp = *x;
    bool has_prev = false;
    seq_t prev = 0;

    while (tmp > 0) {
        seq_t digit = tmp % 7;
        if (has_prev) {
            if ((prev == 6 && digit == 2) || (prev == 2 && digit == 6))
                return true;
        }
        prev = digit;
        has_prev = true;
        tmp /= 7;
    }
    return false;
}

/* Return the n-th term of the sequence (offset 1).  Calls are
   memoized so that any order of n is supported. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behavior below the offset is irrelevant */

    /* ---------- cache of already found terms ---------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* largest integer examined so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;                     /* next candidate k */

        seq_t next = last_checked + 1;
        if (has_adj26(&last_checked) && !has_adj26(&next)) {
            /* grow cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0) new_cap = 16;
                cache = mem_resize(seq_t, cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
