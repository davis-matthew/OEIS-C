/*
 * A039580  Numbers whose base-6 representation has the same number of 0's,
 *          3's and 4's.
 * https://oeis.org/A039580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑6 representation has the same number of 0’s,
 *            3’s and 4’s. The function searches for the nth such number by
 *            enumerating candidates
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;     /* how many terms we have seen so far */
    seq_t num   = 1;     /* next candidate integer (start at 1) */

    while (found < n)
    {
        unsigned cnt0 = 0, cnt3 = 0, cnt4 = 0;
        seq_t x = num;

        /* Count 0,3,4 digits in the base‑6 representation of num. */
        while (x > 0)
        {
            int d = (int)(x % 6);   /* d is 0–5 */
            if (d == 0) ++cnt0;
            else if (d == 3) ++cnt3;
            else if (d == 4) ++cnt4;
            x /= 6;
        }

        if (cnt0 == cnt3 && cnt3 == cnt4)
        {
            ++found;
            if (found == n)
                return num;   /* nth term found */
        }
        ++num;                 /* try next integer */
    }

    /* The function always returns before reaching this point
       because 'n' is positive and the sequence is infinite. */
    return 0;
}
