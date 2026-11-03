/*
 * A044594  Numbers n such that string 3,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044594:
    a(n) = the nth positive integer n such that      \
           the base‑8 (octal) representation of n contains the substring "32",   \
           but the representation of n+1 does NOT contain "32".
    The offset is 1.  a(1) = 26, a(2) = 90, ...
*/

#include <stdbool.h>

static bool has_32_in_oct(seq_t v)
{
    /* Return true if the octal representation of v contains the
       consecutive digits 3 followed by 2.  The digits are examined
       from least significant to most significant, so we keep a
       record of the previous (lower place) digit and check if the
       current (higher place) digit is 3 and the previous is 2. */
    if (v < 8)              /* less than two octal digits */
        return false;
    seq_t prev = v % 8;     /* least significant digit */
    v /= 8;
    while (v > 0) {
        seq_t cur = v % 8;  /* next higher digit */
        if (cur == 3 && prev == 2)
            return true;
        prev = cur;
        v /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Scan natural numbers until the nth term satisfying the
       definition is found.  The loop runs independently for
       every call – it works for any n in any order. */
    idx_t count = 0;
    for (seq_t x = 1; ; ++x) {
        if (has_32_in_oct(x) && !has_32_in_oct(x + 1)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
