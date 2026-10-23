/*
 * A039253  Numbers whose base-12 representation has the same number of 4's
 *          and 11's.
 * https://oeis.org/A039253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base‑12 representation
 *            contains an equal number of digits 4 and 11. The sequence is
 *            0‑based: a(1)=0, a(2)=1,
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* not defined by OEIS but safeguard */
        return (seq_t)0;

    idx_t count = 0;          /* how many qualifying numbers found */
    idx_t cur   = 0;          /* current integer to test */

    for (;; ++cur)
    {
        seq_t val = (seq_t)cur;
        int  c4  = 0, c11 = 0;

        /* special case: val == 0 has no digits 4 or 11 */
        if (val == 0)
        {
            /* do nothing */
        }
        else
        {
            seq_t temp = val;
            while (temp != 0)
            {
                uint64_t d = (uint64_t)(temp % (seq_t)12);
                if (d == 4)   ++c4;
                else if (d == 11) ++c11;
                temp /= (seq_t)12;
            }
        }

        if (c4 == c11)
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
