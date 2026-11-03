/*
 * A039109  Numbers whose base-9 representation has the same number of 6's and
 *          7's.
 * https://oeis.org/A039109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9868 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   a(n) is the nth smallest non‑negative integer whose base‑9
   representation contains an equal number of digits 6 and 7.
   This code searches integers in increasing order, stores each one
   that satisfies the property in a cached array, and returns the
   required term.  The cache allows repeated calls to be fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether a number has equal counts of 6’s and 7’s in base 9. */
static bool equal_6_7(seq_t x)
{
    int cnt6 = 0, cnt7 = 0;
    if (x == 0) return true;
    while (x > 0) {
        seq_t d = x % (seq_t)9;
        if (d == (seq_t)6) cnt6++;
        else if (d == (seq_t)7) cnt7++;
        x /= (seq_t)9;
    }
    return cnt6 == cnt7;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;  /* next number to test */

    if (n < 1) return 0;  /* undefined for n=0, but harmless */

    /* Grow cache if needed */
    while (cache_len < (size_t)n) {
        if (!cache || cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *p = mem_resize(seq_t, cache, new_cap);
            if (!p) { mem_free(cache); cache = NULL; cache_cap = 0; return 0; }
            cache = p;
            cache_cap = new_cap;
        }

        if (equal_6_7(next_candidate))
            cache[cache_len++] = next_candidate;

        ++next_candidate;
    }

    return cache[n - 1];
}
