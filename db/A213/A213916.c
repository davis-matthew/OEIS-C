/*
 * A213916  Sum of all odd numbers in Collatz (3x+1) trajectory of n.
 * https://oeis.org/A213916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of odd numbers in Collatz (3x+1) trajectory of n */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If called with an index below the offset (n==0), the result
       is undefined but we return 0 to avoid an infinite loop. */
    if (n == 0)
        return 0;

    seq_t x = (seq_t)n;
    seq_t sum = 0;

    while (x != 1)
    {
        if ((x & 1) != 0)          /* x is odd */
        {
            sum += x;              /* accumulate odd term */
            x = 3 * x + 1;         /* Collatz step for odd */
        }
        else
        {
            x >>= 1;               /* Collatz step for even (divide by 2) */
        }
    }

    /* x is now 1, which is odd, so include it. */
    sum += 1;
    return sum;
}
