/*
 * A044780  Numbers n such that string 6,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the decimal representation of n contains the
 *            substring "67" but the decimal representation of n+1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: test whether a decimal number x contains the substring 67. */
static bool has67(seq_t x)
{
    while (x >= 10) {
        if ((x % 10) == 7 && ((x / 10) % 10) == 6)
            return true;
        x /= 10;
    }
    return false;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Static state: cache of found terms, current number being scanned,
       and current size/capacity of the cache. */
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;   /* number to start scanning from */

    /* Grow the cache if needed. */
    if (cache_sz >= n)            /* already computed enough terms */
        return cache[n - 1];

    /* Generate terms until we have n of them. */
    while (cache_sz < (size_t)n) {
        /* Scan the next candidate number. */
        ++next_candidate;

        /* Only candidates containing "67" are interesting. */
        if (!has67(next_candidate))
            continue;

        /* But (n+1) must not contain "67". */
        if (has67(next_candidate + 1))
            continue;

        /* Store the found value. */
        if (cache_sz == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *new_arr = mem_new(seq_t, new_cap);
            if (cache && new_arr) {
                mem_copy(new_arr, cache, cache_sz * sizeof(seq_t));
                mem_free(cache);
            }
            cache = new_arr;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = next_candidate;
    }

    return cache[n - 1];
}
