/*
 * A195257  O.g.f.: Sum_{n>=0} 5*(n+5)^(n-1)*x^n/(1+n*x)^n.
 * https://oeis.org/A195257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=1..n} 5^k/(k-1)! , a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    /* compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* first term of the sum: 5^1 * (n-1)! / 0! = 5 * (n-1)! */
    seq_t term = fact * (seq_t)5;
    seq_t sum  = term;

    /* iterate k = 2 .. n, using the recurrence
       term_k = term_{k-1} * 5 / (k-1)  (exact division)       */
    for (idx_t k = 1; k < n; ++k) {
        term = (term * (seq_t)5) / (seq_t)k;
        sum += term;
    }

    return sum;
}
