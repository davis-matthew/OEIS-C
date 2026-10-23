/*
 * A139435  Largest m<n such that first terms of the Collatz trajectory of m
 *          sum up to n; a(n)=0 if no such m exists.
 * https://oeis.org/A139435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m<n such that a prefix of the Collatz trajectory
 *            of m sums to n; 0 if no such m
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, used by idx_t and seq_t in the build */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;     /* a(1) = 0 */

    seq_t target = (seq_t)n;

    for (idx_t m = n - 1; m; m--)
    {
        seq_t sum = 0;
        seq_t cur  = (seq_t)m;

        while (sum < target)
        {
            sum += cur;
            if (sum == target)
                return (seq_t)m;

            /* next Collatz term */
            if (cur == 1)
                cur = 4;
            else if ((cur & 1) == 0)
                cur /= 2;
            else
                cur = cur * 3 + 1;
        }
    }
    return 0;     /* no such m */
}
