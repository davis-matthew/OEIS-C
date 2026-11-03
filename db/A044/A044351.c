/*
 * A044351  Numbers k such that string 1,9 occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Return the n-th number k such that the decimal
   representation of k contains the substring "19" but k-1
   does not.  Sequence A044351, offset 1. */

#include <stdbool.h>

/* Helper: does decimal representation of x contain "19"? */
static bool contains_19(seq_t x)
{
    const seq_t hundred   = 100;
    const seq_t nineteen  = 19;
    const seq_t ten       = 10;
    for (; x >= ten; x /= 10) {
        if ((x % hundred) == nineteen)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for offset 1, but safe */
        return 0;

    seq_t count = 0;
    seq_t k = 19;

    while (1) {
        if (contains_19(k)) {
            seq_t km1 = k - 1;
            if (!contains_19(km1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
}
