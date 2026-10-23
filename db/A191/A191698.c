/*
 * A191698  a(n) = (122n^3 + 140n^2 + 45n + 3n(-1)^n)/8.
 * https://oeis.org/A191698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (122*n^3 + 140*n^2 + 45*n + 3*n*(-1)^n)/8
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    const seq_t nn = n;

    /* Compute the numerator using unsigned arithmetic.  The term 3*n*(-1)^n
       is handled separately for even and odd n to avoid signed arithmetic
       that could underflow. */
    seq_t num;
    if ((n & 1) == 0)   /* n is even: (-1)^n =  1 */
    {
        num = 122 * nn * nn * nn
            + 140 * nn * nn
            +  45 * nn
            +  3 * nn;
    }
    else                /* n is odd: (-1)^n = -1 */
    {
        /* The expression 122*n^3 + 140*n^2 + 45*n is always ≥ 3*n for
           n ≥ 1, so the subtraction cannot underflow. */
        num = 122 * nn * nn * nn
            + 140 * nn * nn
            +  45 * nn
            -  3 * nn;
    }

    /* Divide by 8 (the polynomial is divisible by 8 for all integer n). */
    return num / 8;
}
