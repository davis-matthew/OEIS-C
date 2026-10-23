/*
 * A044471  Numbers n such that string 2,3 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer n (1‑based) such that the base‑5 representation
 * of n contains the substring "23" but the base‑5 representation of n+1
 * does not.  The function scans natural numbers until it has found the
 * requested term.  It works for any seq_t size supported by the
 * framework. */
#include <stdbool.h>

/* Return true if the base‑5 representation of x contains the pattern
 * "23" (a digit 2 immediately followed by a digit 3, with 2 more
 * significant than 3). */
static bool has23(seq_t x)
{
    int prev = -1;          /* previous more‑significant digit */
    seq_t y = x;
    while (y > 0) {
        int dig = (int)(y % 5);   /* least‑significant digit */
        if (dig == 2 && prev == 3)
            return true;
        prev = dig;
        y /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t idx = 0;          /* number of qualifying terms found */
    seq_t num = 0;          /* current candidate integer */

    while (1) {
        ++num;              /* start testing from 1 upward */
        if (has23(num) && !has23(num + 1)) {
            ++idx;
            if (idx == n)
                return num;
        }
    }
}
