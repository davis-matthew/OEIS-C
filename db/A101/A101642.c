/*
 * A101642  a(n) = Knuth's Fibonacci (or circle) product "3 o n".
 * https://oeis.org/A101642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A000201(n+1) + 2*n - 3, where A000201(k) = floor((k +
 *            floor(sqrt(5*k*k)))/2)
 * Range:     u8 n <= 6, u16 n <= 706, u32 n <= 8740, u64 n <= 8740,
 *            u128 n <= 8740, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* sequence is defined for n ≥ 1 */

    /* k = n+1 : argument of the lower Wythoff sequence          */
    seq_t k   = (seq_t)(n + 1);

    /* Compute floor(sqrt(5 * k^2)) – integer square root is provided by the utils library   */
    seq_t ksq = k * k;              /* k^2   */
    seq_t val = ksq * (seq_t)5;     /* 5*k^2 */
    seq_t t   = isqrt(val);         /* floor(sqrt(5*k^2)) */

    /* A000201(k) = floor((k + t)/2)                                      */
    seq_t low = (k + t) / 2;

    /* Final formula: a(n) = 3*low + 2*n - 3                            */
    seq_t nn = (seq_t)n;
    seq_t result = 3 * low + 2 * nn - 3;

    return result;
}
