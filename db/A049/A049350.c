/*
 * A049350  Row sums of triangle A049326.
 * https://oeis.org/A049350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=11, a(3)=71, a(4)=356, a(5)=1866 For n>5: a(n) =
 *            a(n-1) + 10*a(n-2) + 50*a(n-3) + 125*a(n-4) + 125*a(n-5) (This
 *            follows from the generating function x*p(4,x)/(1-x*p(4,x)).)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; undefined below */

    seq_t prev[5] = {0, 0, 0, 0, 0}; /* holds a(n-5) … a(n-1) */
    seq_t cur   = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i == 1)          cur = 1;
        else if (i == 2)     cur = 11;
        else if (i == 3)     cur = 71;
        else if (i == 4)     cur = 356;
        else if (i == 5)     cur = 1866;
        else
        {
            /* a(n) = a(n-1) + 10*a(n-2) + 50*a(n-3) + 125*a(n-4) + 125*a(n-5) */
            cur = prev[4]
                + (seq_t)10 * prev[3]
                + (seq_t)50 * prev[2]
                + (seq_t)125 * prev[1]
                + (seq_t)125 * prev[0];
        }

        /* shift window: drop a(n-5), shift others, insert a(n) as new prev[4] */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = cur;
    }
    return cur;
}
