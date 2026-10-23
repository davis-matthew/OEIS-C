/*
 * A026740  a(n) = 2^n*(2^n - 1)*(2^n - 2)/6.
 * https://oeis.org/A026740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2^n,3) = 2^n*(2^n-1)*(2^n-2)/6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle n=0 separately because 2^0-2 would underflow. */
    seq_t u = pow2(n);
    if (u < 2)         /* only true when n==0 */
        return (seq_t)0;

    seq_t a = u;
    seq_t b = u - (seq_t)1;
    seq_t c = u - (seq_t)2;

    /* Divide by 3 first; one of the three consecutive numbers is a multiple of 3 */
    if (a % (seq_t)3 == 0)
        a /= (seq_t)3;
    else if (b % (seq_t)3 == 0)
        b /= (seq_t)3;
    else
        c /= (seq_t)3;

    /* Divide by 2 next; among the three numbers at least one is even */
    if (a % (seq_t)2 == 0)
        a /= (seq_t)2;
    else if (b % (seq_t)2 == 0)
        b /= (seq_t)2;
    else
        c /= (seq_t)2;

    /* Multiply the remaining factors; all divisions are exact so the product is integer */
    seq_t res = a;
    res *= b;
    res *= c;

    return res;
}
