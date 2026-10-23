/*
 * A179085  Odd numbers having an odd sum of digits in their decimal
 *          representation.
 * https://oeis.org/A179085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = the nth odd integer whose decimal digit sum is odd
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;          /* number of accepted odd numbers found */
    seq_t val = 1;          /* current odd integer to test */

    while (1)
    {
        /* test parity of the digit sum via the utility function */
        seq_t dsum = digit_sum(val);
        if ((dsum & 1) != 0)         /* digit sum odd? */
        {
            ++cnt;
            if (cnt == n)            /* nth required term */
                return val;
        }

        /* move to the next odd integer */
        val += 2;
    }
}
