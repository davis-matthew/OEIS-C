/*
 * A120717  Expansion of x*(67 + 134*x - 287*x^2 -
 *          378*x^3)/((1+2*x)*(1-3*x)*(1 - 5*x - 10*x^2)).
 * https://oeis.org/A120717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) = 6*a(n-1) + 11*a(n-2) -
 *            40*a(n-3) - 60*a(n-4) for n >= 5, with initial values: a(0)=0,
 *            a(1)=67, a(2)=536, a(3)=3666, a(4)=24834
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial terms */
    const seq_t a0 = (seq_t)0;
    const seq_t a1 = (seq_t)67;
    const seq_t a2 = (seq_t)536;
    const seq_t a3 = (seq_t)3666;
    const seq_t a4 = (seq_t)24834;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* buffer holds the last four terms needed for the recurrence:
       b0 = a(n-4), b1 = a(n-3), b2 = a(n-2), b3 = a(n-1)   */
    seq_t b0 = a1;
    seq_t b1 = a2;
    seq_t b2 = a3;
    seq_t b3 = a4;
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i)
    {
        /* compute a(i) = 6*b3 + 11*b2 - 40*b1 - 60*b0 */
        cur = (seq_t)6 * b3 + (seq_t)11 * b2 - (seq_t)40 * b1 - (seq_t)60 * b0;

        /* shift the buffer for next iteration */
        b0 = b1;
        b1 = b2;
        b2 = b3;
        b3 = cur;
    }
    return cur;
}
