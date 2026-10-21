/*
 * A077164  Triangle in which n-th row gives n numbers relatively prime to n
 *          and greater than all earlier terms in the triangle.
 * https://oeis.org/A077164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle where each row r
 *            contains r numbers coprime to r and each number is larger than
 *            every earlier number in the triangle. The function builds rows
 *            until it reaches the desired element, using a search for the
 *            next coprime
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t produced = 0;   /* how many terms have been produced so far */
    seq_t last = 0;       /* largest term produced so far */

    for (idx_t row = 1; ; ++row)            /* rows 1,2,3,... */
    {
        /* start searching just after the larger of last produced and the
           current row index (so that we never re‑use a previous number). */
        seq_t start = (last > row) ? last : row;
        seq_t m = start + 1;

        idx_t found = 0;                     /* how many in this row found */
        while (found < row)
        {
            if (gcd(m, (seq_t)row) == 1)     /* m coprime to row */
            {
                ++found;
                ++produced;
                if (produced == n)           /* the desired term */
                    return m;
            }
            ++m;
        }
        /* after row is filled, last becomes the largest number in that row */
        last = m - 1;
    }
}
