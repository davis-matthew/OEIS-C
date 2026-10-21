/*
 * A061476  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 7.
 * https://oeis.org/A061476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alt_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose alternating digit sum (leftmost digit minus the next,
   plus the next, etc.) equals 7.  */
#include <stdlib.h>
#include "../utils/alt_sum.h"

seq_t a(idx_t n)
{
    /* Cache the list of terms that have already been found. */
    static seq_t *cache = NULL;   /* dynamically growing array */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next = 1;        /* next integer to test */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Allocate / grow storage for new terms if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;                     /* allocation failure, unlikely */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t cand = (seq_t)next;
        if (alt_sum(cand) == 7) {
            cache[cache_len++] = cand;
        }
        ++next;
    }

    return cache[n - 1];
}
