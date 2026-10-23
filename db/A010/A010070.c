/*
 * A010070  Base 8 self or Colombian numbers (not of form k + sum of base 8
 *          digits of k).
 * https://oeis.org/A010070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 7905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Self numbers in base 8: numbers not expressible as
   k + digit_sum_base8(k).  The function returns the n‑th such
   number (1‑based index).  Uses a growing bitmap to mark all
   numbers that have appeared as k + digit_sum_base8(k) and
   a dynamic array to store the self numbers found. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/digit_sum8.h"

seq_t a(idx_t n)
{
    /* State retained between calls so that any order of n is
       handled correctly. */
    static idx_t processed_m = 0;   /* last processed m (>=0) */
    static idx_t self_count  = 0;   /* number of self numbers found */
    static size_t self_cap   = 0;   /* capacity of self_vals array */
    static seq_t *self_vals  = NULL;/* array of self numbers */

    /* Bitmap: one byte per integer marking whether it has been
       generated as k + digit_sum8(k).  Size grows on demand. */
    static unsigned char *bitmap    = NULL;
    static size_t  bitmap_cap   = 0;/* number of slots in bitmap */

    /* Generate self numbers until we have at least n of them. */
    while (self_count < n) {
        /* Next integer to test */
        processed_m++;
        seq_t m = (seq_t)processed_m;

        /* The integer that would generate m (if any). */
        seq_t d = m + digit_sum8(m);

        /* Ensure the bitmap covers index 'd'.  d is a seq_t but we
           will use it as a size_t index; in practice for n ≤ 10 000
           it always fits into size_t. */
        size_t d_idx = (size_t)d;
        if (d_idx >= bitmap_cap) {
            size_t new_cap = bitmap_cap ? bitmap_cap * 2 : 64;
            while (new_cap <= d_idx) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(bitmap, new_cap);
            if (!tmp) abort();     /* out of memory */
            /* zero the newly allocated part */
            memset(tmp + bitmap_cap, 0, new_cap - bitmap_cap);
            bitmap = tmp;
            bitmap_cap = new_cap;
        }

        /* Mark that d has been generated. */
        bitmap[d_idx] = 1;

        /* If m has never been generated, it is a self number. */
        if (bitmap[(size_t)m] == 0) {
            /* Grow the array of self numbers if needed. */
            if ((size_t)self_count == self_cap) {
                size_t new_cap = self_cap ? self_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(self_vals, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                self_vals = tmp;
                self_cap = new_cap;
            }
            self_vals[self_count++] = m;
        }
    }

    /* Return the requested self number (1‑based). */
    return self_vals[n - 1];
}
