/*
 * A039419  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 6's.
 * https://oeis.org/A039419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   the sequence of positive integers whose decimal representation
 *            contains the same non‑zero number of the digits 3 and 6
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_equal_nonzero_36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_nonzero_36.h"

/* Return the n-th element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static size_t len = 0;    /* number of terms already computed */
    static size_t cap = 0;    /* allocated capacity of cache */
    static seq_t next  = 1;   /* next number to test */

    /* extend cache until we have at least n terms */
    while (len < n) {
        while (!has_equal_nonzero_36(next)) ++next;   /* skip non‑valid numbers */

        /* store the found term */
        if (len == cap) {
            cap = cap ? cap * 2 : 8;                  /* grow capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = next;
        ++next;   /* advance for the next search */
    }

    return cache[n - 1];
}
