/*
 * A176364  y-values in the solution to x^2-61*y^2=1.
 * https://oeis.org/A176364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3532638098*a(n-1) - a(n-2) with a(1)=0, a(2)=226153980
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)226153980;

    /* Recurrence constants.  Use constants that fit in standard types. */
    const seq_t COEFF = (seq_t)3532638098U;   /* 2 * fundamental x = 2*1766319049 */
    seq_t y_prev2 = (seq_t)0;                /* a(1)  */
    seq_t y_prev1 = (seq_t)226153980;        /* a(2)  */

    /* Iterate from 3 up to n (inclusive).  idx_t is unsigned, so using
       a loop counter of type idx_t is safe. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t y_curr = COEFF * y_prev1 - y_prev2;
        y_prev2 = y_prev1;
        y_prev1 = y_curr;
    }

    return y_prev1;
}
