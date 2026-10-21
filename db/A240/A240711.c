/*
 * A240711  Sum of the largest parts in the partitions of 4n into 4 parts with
 *          smallest part = 1.
 * https://oeis.org/A240711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over all partitions of 4n into 4 positive integers
 *            with smallest part = 1 of the largest part. This is equivalent
 *            to summing the largest part over all partitions of 4n-1 into 3
 *            parts. The following implementation does so by iterating over
 *            the possible largest part x and counting the ad...
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute M = 4n - 1 */
    seq_t M = (seq_t)n * 4 - 1;

    /* lower bound for the largest part: ceil(M/3) */
    idx_t x_start = (idx_t)((M + 2) / 3);
    /* upper bound: when the two smaller parts are both 1, x = M-2 */
    idx_t x_max   = (idx_t)(M - 2);

    seq_t result = 0;

    for (idx_t x = x_start; x <= x_max; ++x)
    {
        seq_t S = M - (seq_t)x;              /* sum of the two smaller parts */
        seq_t floorHalf = S / 2;             /* floor(S/2) */

        /* lower bound for the smallest part z:
           must satisfy z >= 1 and z >= S - x (to keep y <= x) */
        seq_t l = (S > (seq_t)x) ? (S - (seq_t)x) : 1;
        if (l < 1) l = 1;

        seq_t count = 0;
        if (l <= floorHalf)
            count = floorHalf - l + 1;

        result += (seq_t)x * count;
    }

    return result;
}
