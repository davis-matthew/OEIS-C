/*
 * A043116  Numbers k such that 0 and 1 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the base‑6 *
 *            representation of k contains an adjacent pair of the digits 0
 *            and 1 * (in either order, i.e. the substrings "01" or "10") but
 *            the * representation of k‑1 does not contain such a pair
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_adjacent_0_1(idx_t x)
{
    int prev = -1;          /* previous (higher significance) digit */
    while (x > 0) {
        int d = x % 6;
        if (prev != -1) {
            if ((prev == 0 && d == 1) || (prev == 1 && d == 0))
                return true;
        }
        prev = d;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {              /* iterate over positive integers */
        if (has_adjacent_0_1(k) && !has_adjacent_0_1(k - 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
