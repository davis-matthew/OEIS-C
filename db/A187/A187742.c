/*
 * A187742  G.f.: Sum_{n>=0} (n+x)^n * x^n / (1 + n*x + x^2)^n.
 * https://oeis.org/A187742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+n+2)*(n-1)!/2 , with a(0)=a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: compute n! in seq_t by simple integer multiplication.
   Works for builtin unsigned types as well as arbitrary‑precision
   integer classes (which provide operator* and operator/). */
static seq_t factorial(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        res = res * (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;
    seq_t poly   = n_seq * n_seq + n_seq + (seq_t)2;   /* n^2 + n + 2 */
    seq_t fact   = factorial(n - 1);                    /* (n-1)!   */
    seq_t res    = poly * fact;                         /* numerator */
    res = res / (seq_t)2;                               /* divide by 2 */
    return res;
}
