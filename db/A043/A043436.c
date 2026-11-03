/*
 * A043436  Numbers having four 3's in base 8.
 * https://oeis.org/A043436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers with exactly four 3's in base‑8 (octal). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return true iff v has exactly four octal digits equal to 3. */
static bool has_four_3(idx_t v)
{
    unsigned cnt = 0;   /* count of digit 3 encountered */
    while (v) {
        int d = v % 8;
        if (d == 3) {
            if (++cnt > 4)   /* more than four 3's -> reject */
                return false;
        }
        v /= 8;
    }
    return cnt == 4;     /* exactly four 3's */
}

/* Dynamic memoisation of the sequence. */
static seq_t  *cache = NULL;     /* array of terms */
static size_t   cache_sz = 0;    /* number of filled terms   */
static size_t   cache_cap = 0;    /* allocated capacity      */
static idx_t    next_checked = 0;/* next integer to test    */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but harmless */

    while (cache_sz < n) {
        if (has_four_3(next_checked)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort(); /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)next_checked;
        }
        ++next_checked;
    }
    return cache[n - 1];
}
