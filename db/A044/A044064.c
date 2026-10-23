/*
 * A044064  Numbers k such that string 0,3 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that the base‑4 representation
 *            of k contains the adjacent digit pair 0,3, but the
 *            representation of k-1 does not
 * Range:     u8 n <= 18, u16 n <= 4356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_03(seq_t x)
{
    /* Return true if base‑4 representation of x contains "03".
       The function scans least significant digits first. */
    seq_t tmp = x;
    while (tmp >= 4) {
        seq_t low  = tmp % 4;   /* current least significant digit   */
        tmp /= 4;
        seq_t high = tmp % 4;   /* next digit to the left             */
        if (high == 0 && low == 3)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   k     = 1;       /* start from 1, because 0 has no two digits */

    while (1) {
        if (has_03(k) && !has_03(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
