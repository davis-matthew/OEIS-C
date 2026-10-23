/*
 * A039128  Numbers whose base-10 representation has the same number of 2's
 *          and 3's.
 * https://oeis.org/A039128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base‑10 representation has the same
 * number of 2's and 3's.  a(1) = 0.
 * The function scans integers starting from 0 until it
 * has found the requested term.  It works for any seq_t
 * and idx_t type chosen by the build. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) = 0, so handle n == 1 explicitly */
    if (n == 1) return (seq_t)0;

    seq_t value = 0;          /* current integer to test   */
    idx_t  found = 0;         /* how many terms we have  found */

    while (found < n)
    {
        /* Count the digits 2 and 3 of the current value.  Use
           idx_t arithmetic by casting the seq_t value; the
           numbers needed for n up to 10 000 are far below
           2^64. */
        idx_t c2 = 0, c3 = 0;
        idx_t temp = (idx_t)value;

        /* special case: 0 has no digits */
        if (temp != 0)
        {
            while (temp != 0)
            {
                idx_t d = temp % 10;
                if (d == 2) ++c2;
                else if (d == 3) ++c3;
                temp /= 10;
            }
        }

        if (c2 == c3)
        {
            ++found;
            if (found == n) return value;
        }
        ++value;
    }

    return (seq_t)0;   /* unreachable, but keeps the compiler happy */
}
