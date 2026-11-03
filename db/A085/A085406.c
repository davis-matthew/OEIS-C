/*
 * A085406  Duplicate of A022340.
 * https://oeis.org/A085406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = smallest even integer with no two consecutive 1's in *
 *            its binary representation, enumerated in increasing order. *
 *            a(0) = 0, a(1) = 2, a(2) = 4, a(3) = 8,
 * Range:     i8 n <= 20, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a non‑negative integer x has two consecutive 1's
 * in its binary expansion.  For seq_t types (signed/unsigned) we
 * treat X as an unsigned pattern; the check works for signed values
 * as well because we only look at bits. */
static bool has_consecutive_ones(seq_t x)
{
    /* shift right until no bits left */
    while (x) {
        /* if the lowest two bits are both 1, return true */
        if ((x & 3) == 3)
            return true;
        x >>= 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many qualifying numbers we have seen   */
    seq_t val   = 0;      /* current even candidate */

    while (1) {
        /* val is always even because we start at 0 and add 2 each step */
        if (!has_consecutive_ones(val)) {
            if (count == n)
                return val;       /* found the nth term           */
            ++count;
        }
        val += 2;               /* next even number               */
    }
}
