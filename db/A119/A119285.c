/*
 * A119285  Alternating sum of the fourth powers of the first n Fibonacci
 *          numbers.
 * https://oeis.org/A119285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=1..n} (-1)^k F(k)^4 where F(k) is the k-th Fibonacci
 *            number (F(0)=0, F(1)=1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* Sum over k=1..n of (-1)^k * F(k)^4.  The Fibonacci numbers are
       generated iteratively: F(0)=0, F(1)=1, F(k)=F(k-1)+F(k-2). */
    seq_t sum = 0;          /* running total */
    seq_t f_prev = 0;       /* F(k-1) */
    seq_t f_curr = 1;       /* F(k)   */
    seq_t sign = -1;        /* (-1)^k, start with k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute F(k)^4 */
        seq_t p = f_curr * f_curr;   /* F(k)^2  */
        p = p * p;                   /* (F(k)^2)^2 = F(k)^4 */

        /* Add or subtract this term from the sum */
        sum += sign * p;

        /* Advance Fibonacci numbers */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;

        /* Flip the sign for the next term */
        sign = -sign;
    }

    return sum;
}
