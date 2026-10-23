/*
 * A043092  Numbers in which every string of 2 consecutive base 6 digits
 *          contains exactly 2 distinct numbers.
 * https://oeis.org/A043092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   has_no_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043092
 *
 *  A(n) = n-th non‑negative integer whose base‑6 representation
 *  contains no pair of identical adjacent digits.
 *  The sequence begins:
 *      0,1,2,3,4,5,6,8,9,10,11,12,13,15,16,17,18,19,20,22,23,24,25,26,...
 *
 *  Algorithm:
 *  - Maintain a static cache of already computed values.
 *  - Keep a running candidate integer starting from 0.
 *  - When the cache contains fewer than n values,
 *    test successive candidates for the “no equal adjacent digits” property
 *    and store every qualifying number.
 *  - Finally return cache[n‑1].
 *
 *  Complexity:
 *  O(n) time and O(n) storage for the first n terms.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_no_adjacent.h"

#if !defined(MAX_CACHE)
#   define MAX_CACHE 200000   /* enough for n up to 100 000 */
#endif

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t next_candidate = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; below offset is irrelevant */

    /* Extend the cache until it contains at least n items. */
    while (cache_len < n) {
        if (has_no_adjacent(next_candidate)) {
            if (cache_len < MAX_CACHE) {
                cache[cache_len++] = next_candidate;
            }
            /* if we exceed MAX_CACHE we simply stop producing further values,
               but for all practical n in this problem this never happens. */
        }
        next_candidate++;
    }
    return cache[n - 1];
}
