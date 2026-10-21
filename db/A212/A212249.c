/*
 * A212249  Number of (w,x,y,z) with all terms in {1,...,n} and 3w<x+y+z+n.
 * https://oeis.org/A212249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (59*n^4 -10*n^3 +5*n^2 -6*n -8*((((n+1) mod
 *            3)+(-1)^((n+1) mod 3))*(-1)^(n mod 3)))/72
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t once */
    seq_t nt = (seq_t)n;

    /* power terms */
    seq_t n2 = nt * nt;          /* n^2 */
    seq_t n3 = n2 * nt;          /* n^3 */
    seq_t n4 = n3 * nt;          /* n^4 */

    /* polynomial terms */
    seq_t term1 = (seq_t)59 * n4;   /* 59*n^4 */
    seq_t term2 = (seq_t)10 * n3;   /* 10*n^3 */
    seq_t term3 = (seq_t)5  * n2;   /* 5*n^2 */
    seq_t term4 = (seq_t)6  * nt;   /* 6*n */

    /* compute the small C term:
       C = (((n+1) mod 3) + (-1)^((n+1) mod 3)) * (-1)^(n mod 3)
       Its possible values are -3, -1, 0, 1, 3.       */
    idx_t r = (n + 1) % 3;
    idx_t t = n % 3;
    int r_val = (int)r;
    int t_val = (int)t;
    int s = r_val + ((r_val % 2 == 0) ? 1 : -1);
    int C   = s * ((t_val % 2 == 0) ? 1 : -1);

    /* accumulate numerator in unsigned arithmetic.
       Treat the -8*C part according to the sign of C. */
    seq_t num = term1;
    num -= term2;
    num += term3;
    num -= term4;
    if (C >= 0) {
        num -= (seq_t)8 * (seq_t)C;
    } else {
        num += (seq_t)8 * (seq_t)(-C);
    }

    /* division by 72 gives the integer result. */
    return num / (seq_t)72;
}
