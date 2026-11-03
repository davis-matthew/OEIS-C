/*
 * A074161  Smallest multiple of n (>n) using all composite digits (4,6,8,9
 *          and 0).
 * https://oeis.org/A074161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A074161 sequence:
 *  a(n) = smallest multiple of n (> n) that uses only composite digits
 *  {0,4,6,8,9}.  The algorithm scans successive multiples of n until it
 *  finds one whose decimal representation contains only these digits.
 */

#include <stdbool.h>

/* Test whether all decimal digits of x are in {0,4,6,8,9}. */
static bool composite_digits_ok(seq_t x)
{
    if (x == 0) return true;  /* 0 contains only allowed digits */
    while (x > 0) {
        int d = (int)(x % 10);
        if (!(d == 0 || d == 4 || d == 6 || d == 8 || d == 9))
            return false;
        x /= 10;
    }
    return true;
}

/* a(idx_t n):  compute the smallest multiple of n (> n) that uses only
 * composite digits (0,4,6,8,9). */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2;        /* first multiple strictly greater than n */
    while (true) {
        if (composite_digits_ok(m))
            return m;
        m += (seq_t)n;              /* next multiple of n */
    }
}
