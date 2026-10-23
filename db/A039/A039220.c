/*
 * A039220  Numbers whose base-12 representation has the same number of 1's
 *          and 2's.
 * https://oeis.org/A039220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_1_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force scan of numbers whose base‑12 representation
   has the same number of digits 1 and 2.  The function memoizes the
   sequence for arbitrary‑precision or 64‑bit builds.  a(1) = 0. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_1_2.h"

static seq_t *found = NULL;         /* dynamic array of terms */
static size_t found_len = 0;        /* number of terms stored      */
static size_t found_cap = 0;        /* capacity of the array       */
static seq_t last_scanned = (seq_t)(-1); /* last number examined */

/* Grow the array to hold at least min_cap elements.  */
static void grow_found(size_t min_cap)
{
    size_t new_cap = found_cap ? found_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    found = tmp;
    found_cap = new_cap;
}

/* Return the n-th term of the sequence (a(1)=0).  The result is memoized
   so that calls with arbitrary n order remain fast. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset, but safe for C/C++ */

    /* Compute terms until we have at least n of them. */
    while (found_len < (size_t)n) {
        seq_t candidate = last_scanned + 1;   /* next number to test */
        if (has_equal_1_2(candidate)) {
            if (found_len + 1 > found_cap) grow_found(found_len + 1);
            found[found_len++] = candidate;
        }
        last_scanned = candidate;
    }

    return found[n - 1]; /* zero‑based indexing */
}
