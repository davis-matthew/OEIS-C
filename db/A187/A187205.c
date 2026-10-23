/*
 * A187205  Numbers such that the last of the absolute differences of divisors
 *          is 0.
 * https://oeis.org/A187205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_zero_div_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187205: Numbers such that the last of the absolute differences
   of their sorted divisors is 0.  The function a(n) returns the n‑th such
   number (1‑based).  The value is computed by brute‑force search using
   an efficient divisor enumeration and iterative difference reduction. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_zero_div_diff.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms, grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t cnt   = 0;
    static uint64_t next = 1;

    /* Grow cache and search forward until we have at least n terms. */
    while (cnt < n) {
        /* Find next qualifying number. */
        while (!has_zero_div_diff(next)) ++next;

        /* Append to cache, resizing if necessary. */
        if (cnt >= cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp    = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }
        cache[cnt++] = (seq_t)next;
        ++next;   /* advance candidate for next iteration */
    }

    return cache[n - 1];
}
