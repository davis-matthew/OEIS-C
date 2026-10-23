/*
 * A135140  Numbers n such that n and n^2 do not contain successive identical
 *          digits.
 * https://oeis.org/A135140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the nth integer (n ≥ 1) such that the integer and its
   square contain no consecutive equal digits.  The sequence begins
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 14, 16, 17, 18, 19, 23, 24, 25,
   26, 27, 28, 29, 31, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/no_consecutive_equal_digits.h"

seq_t a(idx_t n)
{
    static seq_t next_candidate = 0;
    static seq_t *cache         = NULL;
    static size_t cache_len     = 0;
    static size_t cache_cap     = 0;

    if (n == 0) return 0;            /* index offset is 1; not used */

    /* Extend the cache until it holds at least n terms. */
    while (cache_len < n) {
        /* Verify candidate and its square. */
        if (no_consecutive_equal_digits(next_candidate)) {
            seq_t sq = next_candidate * next_candidate;
            if (no_consecutive_equal_digits(sq)) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();           /* allocation failure */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
            }
        }
        next_candidate++;
    }

    return cache[n - 1];
}
