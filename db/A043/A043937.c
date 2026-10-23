/*
 * A043937  Numbers k such that 0 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: numbers k such that the octal representation of k contains
   the adjacent digits 0 and 6 (in either order) but the octal representation
   of k+1 does not.  The function searches sequentially from 0. */

#include <stdbool.h>

/* Return true if x contains the adjacent digits 0 and 6 (in any order) in base‑8. */
static bool has_06(seq_t x)
{
    if (x < 8) return false;          /* less than two digits, cannot contain two adjacent digits */
    seq_t prev = 8;                    /* sentinel value that is neither 0 nor 6 */
    while (x > 0) {
        seq_t d = x % 8;
        if ((prev == 6 && d == 0) || (prev == 0 && d == 6))
            return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* defined for offset 1; below offset behaviour is irrelevant */
    idx_t count = 0;
    seq_t k = 0;
    while (1) {
        if (has_06(k) && !has_06(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
