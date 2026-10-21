/*
 * A091095  Expansion of (1+4*x-24*x^2)/((1-4*x)(1+4*x)).
 * https://oeis.org/A091095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4^n if n > 0 is odd * - (4^n)/2 if n > 0 is even * 1 if n
 *            == 0 * (This matches a(n) = (4^n - 3*(-4)^n)/4 for n > 0, with
 *            a(0)=1.)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow4(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;   /* No effect when exp becomes 0 */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* For n > 0 */
    if (n & 1)         /* odd */
        return pow4(n);
    else {             /* even */
        seq_t val = pow4(n);
        val /= (seq_t)2;   /* divide by 2 */
        return -(val);     /* make negative */
    }
}
