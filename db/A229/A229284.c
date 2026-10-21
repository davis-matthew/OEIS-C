/*
 * A229284  Number of ascending runs in {1,...,10}^n.
 * https://oeis.org/A229284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(n-1)*(11*n+9)/2 for n>0, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 10^(n-1) */
    seq_t pow10_n1 = pow10(n - 1);

    /* (11*n + 9) */
    seq_t mult = (seq_t)(11 * n + 9);

    /* Compute the product without overflow by dividing one factor by 2 first.
       For n>1, pow10_n1 is even (contains a factor of 10).  For n==1,
       mult is even.  Thus the division is exact and we avoid the overflow
       that occurs if we multiply before dividing. */
    if (pow10_n1 & 1u)          /* pow10_n1 is odd -> n == 1 */
        return pow10_n1 * (mult >> 1);
    else
        return (pow10_n1 >> 1) * mult;
}
