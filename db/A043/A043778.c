/*
 * A043778  Numbers n such that number of runs in base 3 representation of n
 *          is congruent to 2 mod 4.
 * https://oeis.org/A043778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   runs_in_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A043778 (offset 1):
   “Numbers n such that number of runs in the base‑3 representation of n
   is congruent to 2 mod 4.”  The function searches forward from the last
   found number, caching results so repeated calls are fast.  The search
   is O(n), well within the 2 s limit for n up to ~10 000. */

#include <stdlib.h>
#include "../utils/runs_in_base3.h"

/* Return the n‑th term (offset 1) of sequence A043778. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the sequence; return 0 for safety */
        return (seq_t)0;

    /* Cache of known terms. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The smallest candidate that we have not examined yet. */
    static seq_t next_candidate = 1;

    /* Grow the cache until it contains at least n entries. */
    while (cache_len < n) {
        /* Find the next qualifying number. */
        while (1) {
            if (runs_in_base3(next_candidate) % 4 == 2) {
                /* Append to cache. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Allocation failure; terminate. */
                        exit(1);
                    }
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                ++next_candidate;    /* advance for next search */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
