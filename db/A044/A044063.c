/*
 * A044063  Numbers n such that string 0,2 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑4 * representation
 *            contains the adjacent digits 0 followed by 2 * (read from most
 *            significant to least), while the previous * integer does not.
 *            The function searches forward until it * finds the desired term
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_0_2(seq_t x)
{
    /* Return true iff the base‑4 representation of x contains
     * the substring “0,2” (most significant to least).  The
     * routine computes the most significant 4‑power first,
     * then scans the digits in natural order. */
    if (x == 0)
        return false;

    /* Find the highest power of 4 <= x.  Avoid overflow by
     * testing against x / 4 before the multiplication. */
    seq_t p = 1;
    while (p <= x / 4)
        p *= 4;

    int prev = -1;          /* no previous digit yet */
    while (p > 0)
    {
        seq_t digit = x / p;   /* current digit */
        x %= p;                /* remainder for the rest */
        if (prev == 0 && digit == 2)
            return true;
        prev = (int)digit;
        p /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Offset 1: n == 0 is outside the OEIS definition; return 0. */
    if (n == 0)
        return 0;

    seq_t val = 1;       /* candidate integer to test */
    idx_t count = 0;     /* number of qualifying integers seen */

    for (;; ++val)
    {
        if (has_0_2(val) && !has_0_2(val - 1))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
