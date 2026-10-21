/*
 * A060813  Numbers whose square has more than 2/3 of its digits the same.
 * https://oeis.org/A060813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose square has more than 2/3 of its digits the same.
   The function a(n) returns the nth term (a(1)=0), using a lazy sieve
   that tests successive integers until the required count is reached. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/good.h"

/* Lazy storage for the sequence values. */
static seq_t  *seq = NULL;      /* array of values */
static size_t  cap = 0;         /* allocated capacity */
static size_t  len = 0;         /* number of stored terms */
static seq_t   last = 0;        /* last candidate examined */

/* Helper to append a value to the dynamic array.  Resizes if necessary. */
static void append(seq_t val)
{
    if (len == cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
    seq[len++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (len == 0) {
        /* Initialise with the first term a(1)=0. */
        append((seq_t)0);
        last = 0;
    }
    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        last++;
        if (good(last))
            append(last);
    }
    return seq[(size_t)n - 1];
}
